#include "includes.h"

void respond(int newsockfd)
{
	char *clientmessage = (char *)malloc(USHRT_MAX * sizeof(char));
	memset(clientmessage, '\0', USHRT_MAX * sizeof(char));
	int msglen = recv(newsockfd, clientmessage, USHRT_MAX * sizeof(char), 0);
	
	if (msglen < 0)
	{
		printf("recv() error\n");
	}
	else if (msglen == 0)
	{
		printf("Client disconnected unexpectedly\n");
	}
	else
	{
		printf("Here is what they have to say: \n%s", clientmessage);
			
		char *method = strtok(clientmessage,  " \t\r\n");
		char *uri = strtok(NULL, " \t");
		char *args;
		char *protocol = strtok(NULL, " \t\r\n");
		
		if (strchr(uri, '?') != NULL)	// This (probably) means we have a GET request, hence we split it between the actual script name and its arguments
		{
			char *tmp = strtok(uri, "?");
			args = strtok(NULL, " \t");
			strcpy(uri, tmp);
		}
		else
		{
			args = NULL;
		}
				
		if (strcmp(uri, "/") == 0)
		{
			uri = (char *)malloc(strlen("/index.php ") * sizeof(char));
			strcpy(uri, "/index.php");
		}
		
		// Look for our file in the folder
		if ( access(&uri[1], F_OK) == -1)
		{
			if (errno == EACCES)		// Permission error
			{
				printf("Access denied to %s, sending 403\n", &uri[1]);
				serveError(403, newsockfd);
			}
			else if (errno == ENOENT)	// File not found
			{
				printf("Resource %s not found, sending 404\n", &uri[1]);
				serveError(404, newsockfd);
			}
		}
		else
		{
			printf("Found resource %s\n", &uri[1]);
			
			char *ext = strrchr(&uri[1], '.');
			
			char *html[] = { ".html", ".htm"};
			char *img[] = { ".png", ".jpg", ".jpeg", ".gif", ".tiff" };
			
			// Do we need PHP ?
			if (strcmp(ext, ".php") == 0)
			{					
				// Was the program compiled with support for PHP calls ?
				if (USE_PHP == 1)
				{
					// Do we *have* PHP ?!
					if ( access("/usr/bin/php-cgi", F_OK) != -1 )
					{
						servePHP(&uri[1], method, args, newsockfd);
					}
					else
					{
						printf("Error: PHP appears to be missing from the host machine\n");
						serveError(501, newsockfd);
					}
				}
				else
				{
					printf("Error: PHP support has been disabled during compilation\n");
					serveError(501, newsockfd);
				}
			}
			else
			{
				if (strcmp(ext, ".css") == 0)
				{
					serveCSS(&uri[1], newsockfd);
				}
				else if (isElementOf(ext, html, 2))
				{
					serveHTML(&uri[1], newsockfd);
				}
				else if (isElementOf(ext, img, 5))
				{
					serveIMG(&uri[1], ext, newsockfd);
				}
				else
				{
					serveError(403, newsockfd);	// If we reach here, we can assume we don't have the rights to view that file... Though access() should report a permission error long before we get here anyway
				}
			}
		}
	}
	
	free(clientmessage);
	close(newsockfd);
}