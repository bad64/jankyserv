#include "includes.h"

void respond(int n)
{
	char *clientmessage = (char *)malloc(USHRT_MAX * sizeof(char));
	memset(clientmessage, '\0', USHRT_MAX * sizeof(char));
	int msglen = recv(n, clientmessage, USHRT_MAX * sizeof(char), 0);
	
	if (msglen < 0)
		printf("recv() error\n");
	else if (msglen == 0)
		printf("Client disconnected unexpectedly\n");
	else
	{
		printf("Here is what they have to say: \n%s", clientmessage);
			
		char *method = strtok(clientmessage,  " \t\r\n");
		char *uri = strtok(NULL, " \t");
		char *prot = strtok(NULL, " \t\r\n");
		
		if (strcmp(uri, "/") == 0)
		{
			uri = (char *)malloc(strlen("/index.php ") * sizeof(char));
			strcpy(uri, "/index.php");
		}
		
		//Look for our file in the folder
		if ( access(&uri[1], F_OK) == -1)
		{
			if (errno == EACCES)		//Permission error
			{
				printf("Access denied to %s, sending 403\n", &uri[1]);
				serveError(403, n);
			}
			else if (errno == ENOENT)	//File not found
			{
				printf("Resource %s not found, sending 404\n", &uri[1]);
				serveError(404, n);
			}
		}
		else
		{
			printf("Found resource %s\n", &uri[1]);
			
			//Handling snowflake cases
			if (strcmp(&uri[1], "info.php") == 0)
			{
				//getInfo(n);	//Bugged
				serveError(500, n);
			}
			else
			{
				char *ext = strrchr(&uri[1], '.');
				
				char *html[] = { ".html", ".htm"};
				char *img[] = { ".png", ".jpg", ".jpeg", ".gif", ".tiff" };
				
				//Do we need PHP ?
				if (strcmp(ext, ".php") == 0)
				{					
					//Was the program compiled with support for PHP calls ?
					if (USE_PHP == 1)
					{
						//Do we *have* PHP ?!
						if ( access("/usr/bin/php", F_OK) != -1 )
						{
							servePHP(&uri[1], n);
						}
						else
						{
							printf("Error: PHP appears to be missing from the host machine\n");
							serveError(501, n);
						}
					}
					else
					{
						printf("Error: PHP support has been disabled during compilation\n");
						serveError(501, n);
					}
				}
				else
				{
					if (strcmp(ext, ".css") == 0)
					{
						serveCSS(&uri[1], n);
					}
					else if (isElementOf(ext, html, 2))
					{
						serveHTML(&uri[1], n);
					}
					else if (isElementOf(ext, img, 5))
					{
						serveIMG(&uri[1], ext, n);
					}
					else
						serveError(503, n);	//If we reach here, we can assume we don't have the rights to view that file... Though access() should report a permission error anyway.
				}
			}
		}
	}
	
	free(clientmessage);
	close(n);
	printf("Worker has finished !\n");
}