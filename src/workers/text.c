#include "../includes.h"

void serveHTML(char *resource, int newsockfd)
{
	printf("1 ");
	// Get file info
	FILE *html = fopen(resource, "r");

	struct stat st;
	stat(resource, &st);
	unsigned int fsize = st.st_size;
	
	char *buf = (char *)calloc(fsize, sizeof(char));
	fread(buf, fsize, 1, html);
	fclose(html);
	
	char *fsize_s = (char *)calloc(12, sizeof(char));
	sprintf(fsize_s, "%d", fsize);
	
	printf("Size of file is %u bytes", fsize);
	
	char *header = (char *)calloc( strlen("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ") + strlen(fsize_s) + strlen("\nConnection: close\n\n") + 1, sizeof(char) );
	memset(header, '\0', sizeof(header));
	strcat(header, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ");
	strcat(header, fsize_s);
	strcat(header, "\nConnection: close\n\n");
	
	printf(", total response size is %lu bytes\n", strlen(header) + fsize);
	
	char *response = (char *)calloc( (strlen(header) + fsize + 1), sizeof(char) );
	memset(response, '\0', sizeof(response) * sizeof(char));
	strcat(response, header);
	strcat(response, buf);

	printf("Writing response... ");
	if ( write(newsockfd, response, strlen(response)) == 0)
	{
		perror("Could not write response to socket");
	}
	else
	{
		printf("OK (wrote %lu bytes)\n", strlen(response));
	}
	
	if (strlen(response) != (strlen(header) + fsize))
	{
		printf("Warning: there might be an issue with response size (%lu vs (%lu + %d))\n", strlen(response), strlen(header), fsize);
	}
	
	free(response);
	free(header);
	free(buf);
	printf("HTML: My work here is done...\n");
}

void serveCSS(char *resource, int newsockfd)
{
	// Get file info
	FILE *css = fopen(resource, "r");

	struct stat st;
	stat(resource, &st);
	unsigned int fsize = st.st_size;
	
	char *buf = (char *)calloc(fsize, sizeof(char));
	fread(buf, fsize, 1, css);
	fclose(css);
	
	char *fsize_s = (char *)calloc(12, sizeof(char));
	sprintf(fsize_s, "%d", fsize);
	
	printf("Size of file is %u bytes", fsize);
	
	// Building response	
	char *header = (char *)calloc( strlen("HTTP/1.1 200 OK\nContent-Type: text/css\nContent-Length: ") + strlen(fsize_s) + strlen("\nConnection: close\n\n") + 1, sizeof(char) );
	memset(header, '\0', sizeof(header));
	strcat(header, "HTTP/1.1 200 OK\nContent-Type: text/css\nContent-Length: ");
	strcat(header, fsize_s);
	strcat(header, "\nConnection: close\n\n");
	
	printf(", total response size is %lu bytes\n", strlen(header) + fsize);
	
	char *response = (char *)calloc( (strlen(header) + fsize + 1), sizeof(char) );
	memset(response, '\0', sizeof(response) * sizeof(char));
	strcat(response, header);
	strcat(response, buf);

	printf("Writing response... ");
	if ( write(newsockfd, response, strlen(response)) == 0)
	{
		perror("Could not write response to socket");
	}
	else
	{
		printf("OK (wrote %lu bytes)\n", strlen(response));
	}
	
	if (strlen(response) != (strlen(header) + fsize))
	{
		printf("Warning: there might be an issue with response size (%lu vs (%lu + %lu + 1))\n", strlen(response), strlen(header), strlen(buf));
	}
	
	free(response);
	free(header);
	free(buf);
	printf("CSS: My work here is done...\n");
}

void servePHP(char *resource, char *method, char *args, int newsockfd)
{	
	// Quick reminder of what should be fed to php-cgi and how:
	// - GATEWAY_INTERFACE = "CGI/1.1"
	// - REDIRECT_STATUS = 200 (or modify php.ini, your call. Might want to make that a compile time option)
	// - SCRIPT_FILENAME = contents of "resource" variable
	// - REQUEST_METHOD = contents of the "method" variable in double quotes
	// - CONTENT_LENGTH = 
	//		- Length of POST data if... well POST
	//		- Not needed if GET (?)
	// - CONTENT_TYPE = application/x-www-form-urlencoded
	
	// Buffer to hold PHP content size
	char *sbuf;
	if ( ( sbuf = (char *)malloc(strlen("65535") * sizeof(char)) ) == NULL )
	{
		printf("Error: Failed to allocate memory in %s at line %d\n", __FILE__, __LINE__);
		serveError(500, newsockfd);
	}
	memset(sbuf, '\0', sizeof(sbuf));
	
	if (strcmp(method, "GET") == 0)
	{
		if (args != NULL)
		{
			char *command;
			if ( (command = (char *)malloc((strlen("php-cgi -f ") + strlen(resource) + 1 + strlen(args) + 2) * sizeof(char))) == NULL)
			{
				printf("Error: Failed to allocate memory for php invoke in %s at line %d\n", __FILE__, __LINE__);
				serveError(500, newsockfd);
			}
			memset(command, '\0', sizeof(command));
			
			strcat(command, "php-cgi -f ");
			strcat(command, resource);
			strcat(command, " \"");
			strcat(command, args);
			strcat(command, "\"");
			
			char *getsize;
			if ( (getsize = (char *)malloc ((strlen(command) + strlen(" | wc -c") + 1) * sizeof(char))) == NULL)
			{
				printf("Error: Failed to allocate memory for php invoke in %s at line %d\n", __FILE__, __LINE__);
				serveError(500, newsockfd);
			}
			memset(getsize, '\0', sizeof(getsize));
			strcat(getsize, command);
			strcat(getsize, " | wc -c");
			
			// Get size of PHP output
			FILE *execsize;
			if ( (execsize = popen(getsize, "r")) <= 0)
			{
				printf("Error: Failed to get PHP output size in %s at line %d\n", __FILE__, __LINE__);
				serveError(500, newsockfd);
				return;
			}
			
			if (fscanf(execsize, "%s", sbuf) == 0)
			{
				printf("Error: Could not read PHP output size into variable in %s at line %d\n", __FILE__, __LINE__);
				serveError(500, newsockfd);
				return;
			}
			pclose(execsize);
			printf("PHP response size is %d bytes\n", atoi(sbuf));
			
			// Get PHP output			
			FILE *php;
			if ( (php = popen(command, "r")) <= 0)
			{
				printf("Error: Failed to get PHP output in %s at line %d\n", __FILE__, __LINE__);
				serveError(500, newsockfd);
				return;
			}
			
			// Write response			
			char *response;
			if ((response = (char *)malloc( (strlen("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ") \
											+ strlen(sbuf) + strlen("\nConnection: close\n\n") + atoi(sbuf) + 1) * sizeof(char))) == NULL)
			{
				printf("Error: Failed to allocate memory for response in %s at line %d\n", __FILE__, __LINE__);
				serveError(500, newsockfd);
				return;
			}
			memset(response, '\0', sizeof(response));
			strcat(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ");
			strcat(response, sbuf);
			strcat(response, "\nConnection: close\n\n");
			if (fread(&response[strlen(response)], atoi(sbuf), 1, php) == 0)
			{
				printf("Error: Failed to build response in %s at line %d\n", __FILE__, __LINE__);
				serveError(500, newsockfd);
				return;
			}
			else
			{
				printf("Responding with:\n");
				printf("%s\n", response);
				
				printf("Writing response... ");
				if (write(newsockfd, response, strlen(response)) == 0)
				{
					perror("Could not write response to socket");
					serveError(500, newsockfd);
					return;
				}
				else
				{
					printf("OK (wrote %lu bytes)\n", strlen(response));
				}
			}
			
			fclose(php);
			free(response);
			free(getsize);
			free(command);
		}
		else
		{
			char *command;
			if ( (command = (char *)malloc((strlen("php-cgi -f ") + strlen(resource) + 1) * sizeof(char))) == NULL)
			{
				printf("Error: Failed to allocate memory for php invoke in %s at line %d\n", __FILE__, __LINE__);
				serveError(500, newsockfd);
			}
			memset(command, '\0', sizeof(command));
			
			strcat(command, "php-cgi -f ");
			strcat(command, resource);
			
			char *getsize;
			if ( (getsize = (char *)malloc ((strlen(command) + strlen(" | wc -c") + 1) * sizeof(char))) == NULL)
			{
				printf("Error: Failed to allocate memory for php invoke in %s at line %d\n", __FILE__, __LINE__);
				serveError(500, newsockfd);
			}
			memset(getsize, '\0', sizeof(getsize));
			strcat(getsize, command);
			strcat(getsize, " | wc -c");
			
			// Get size of PHP output
			FILE *execsize;
			if ( (execsize = popen(getsize, "r")) <= 0)
			{
				printf("Error: Failed to get PHP output size in %s at line %d\n", __FILE__, __LINE__);
				serveError(500, newsockfd);
				return;
			}
			
			if (fscanf(execsize, "%s", sbuf) == 0)
			{
				printf("Could not read PHP output size into variable in %s at line %d\n", __FILE__, __LINE__);
				serveError(500, newsockfd);
				return;
			}
			pclose(execsize);
			printf("PHP response size is %d bytes\n", atoi(sbuf));
			
			// Get PHP output
			FILE *php;
			if ( (php = popen(command, "r")) <= 0)
			{
				printf("Error: Failed to get PHP output in %s at line %d\n", __FILE__, __LINE__);
				serveError(500, newsockfd);
				return;
			}
			
			// Write response
			char *response;
			if ((response = (char *)malloc( (strlen("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ") \
											+ strlen(sbuf) + strlen("\nConnection: close\n\n") + atoi(sbuf) + 1) * sizeof(char))) == NULL)
			{
				printf("Error: Failed to allocate memory for response in %s at line %d", __FILE__, __LINE__);
				serveError(500, newsockfd);
				return;
			}
			memset(response, '\0', sizeof(response));
			strcat(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ");
			strcat(response, sbuf);
			strcat(response, "\nConnection: close\n\n");
			if (fread(&response[strlen(response)], atoi(sbuf), 1, php) == 0)
			{
				printf("Error: Failed to build response in %s at line %d\n", __FILE__, __LINE__);
				serveError(500, newsockfd);
				return;
			}
			else
			{
				printf("Writing response... ");
				if (write(newsockfd, response, strlen(response)) == 0)
				{
					perror("Could not write response to socket");
					serveError(500, newsockfd);
					return;
				}
				else
				{
					printf("OK (wrote %lu bytes)\n", strlen(response));
				}
			}
			
			fclose(php);
			free(response);
			free(getsize);
			free(command);
		}
	}
	else if (strcmp(method, "POST") == 0)
	{
		
	}
	else	// Unless the user agent is weird, this shouldn't happen, but better safe than sorry
	{
		serveError(501, newsockfd);
	}
	
	printf("PHP: My work here is done...\n");
}