#include "includes.h"

void respond(int newsockfd)
{
	clock_t begin, end;
	double elapsed;
	begin = clock();
	
	char *clientmessage = (char *)malloc(USHRT_MAX * sizeof(char));
	memset(clientmessage, '\0', USHRT_MAX * sizeof(char));
	int msglen = recv(newsockfd, clientmessage, USHRT_MAX * sizeof(char), 0);
	
	char *copymsg = (char *)malloc(strlen(clientmessage) * sizeof(char));	// Copy of the client message to be butchered by strtok() later
	strcpy(copymsg, clientmessage);
	
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
		printf("Here is what they have to say: \n%s\n", clientmessage);
			
		char *method = strtok(copymsg,  " \t\r\n");
		char *uri = strtok(NULL, " \t");
		char *args;
		char *protocol = strtok(NULL, " \t\r\n");
		
		if (strchr(uri, '?') != NULL)	// This (ideally should) means we have a GET request, hence we split it between the actual script name and its arguments
		{
			char *tmp = strtok(uri, "?");
			args = strtok(NULL, " \t");
			strcpy(uri, tmp);
		}
		else
		{
			args = NULL;
		}
		
		if (strcmp(method, "POST") == 0)
		{
			printf("\n");
		}
				
		if (strcmp(uri, "/") == 0)
		{
			// This is temporarily hardcoded and will change whenever I get around to implement proper redirection
			
			printf("Redirect / to index.php\n");
			if ( write(newsockfd, "HTTP/1.1 301 Moved Permanently\r\nLocation: /index.php\r\n\r\n", strlen("HTTP/1.1 301 Moved Permanently\r\nLocation: /index.php\r\n\r\n")) == 0)
			{
				serveError(500, newsockfd);
			}
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
						servePHP(&uri[1], clientmessage, method, args, newsockfd);
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
					serveOctetStream(&uri[1], newsockfd);
				}
			}
		}
	}
	
	end = clock();
	elapsed = (double)(end - begin) / CLOCKS_PER_SEC;
	
	printf("Request served in %.2f seconds\n", elapsed);
	
	free(copymsg);
	free(clientmessage);
	close(newsockfd);
}