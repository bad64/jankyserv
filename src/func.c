#include "includes.h"

void serveError(char * errorcode, int n)
{
	char* phpcommand = (char *)malloc((strlen("php error.php error=") + strlen(errorcode) + 1) * sizeof(char));
	memset(phpcommand, '\0', sizeof(phpcommand));
	strcat(phpcommand, "php error.php error=");
	strcat(phpcommand, errorcode);
	
	FILE *php;
	if ( (php = (FILE *)popen(phpcommand, "r")) <= 0 )
	{
		printf("PHP failed in a truly spectacular way !!");
		
		char *emergency500 = "HTTP/1.1 500 Internal Server Error\nConnection: close\n\n<!DOCTYPE html><html><head><title>Internal Server Error</title></head><body><h1 style=\"text-align: center;\">500 Internal Server Error</h1><hr><p style=\"text-align: center;\">JankyServ v0.1</p></body></html>";
		
		write(n, emergency500, sizeof(emergency500));
	}
	else
	{
		unsigned long fsize = 0;
		char *buf = (char *)calloc(UINT_MAX, sizeof(char));
		memset(buf, '\0', UINT_MAX);
		char c;
		
		while ( (c = fgetc(php)) != EOF)
		{
			buf[fsize] = c;
			fsize++;
		}
		buf[fsize] = '\0';
		
		//Building response
		char *fsize_s = (char *)calloc(256, sizeof(char));
		sprintf(fsize_s, "%lu", fsize);
		printf("Size of file is %s bytes", fsize_s);
		
		char *header = (char *)calloc( strlen("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ") + strlen(fsize_s) + strlen("\nConnection: close\n\n") + 1, sizeof(char) );
		memset(header, '\0', sizeof(header));
		strcat(header, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ");
		strcat(header, fsize_s);
		strcat(header, "\nConnection: close\n\n");
		
		printf(", total response size is %lu bytes\n", strlen(header) + fsize);
		
		char *response = (char *)calloc( (strlen(header) + fsize), sizeof(char) );
		memset(response, '\0', sizeof(response) * sizeof(char));
		strcat(response, header);
		strcat(response, buf);
		
		printf("Writing response...");
		if ( write(n, response, strlen(response)) == 0)
		{
			perror("Could not write response to socket");
		}
		else
		{
			printf("OK (wrote %lu bytes)\n", strlen(response));
		}
		
		if (strlen(response) != (strlen(header) + fsize))
			printf("Warning: there might be an issue with response size (%lu vs (%lu + %lu + 1))\n", strlen(response), strlen(header), strlen(buf));
		
		free(response);
		free(header);
		free(buf);
		fclose(php);
	}
	
	free(phpcommand);
}

void serveHTML(char *resource, int n)
{
	FILE *html = fopen(resource, "r");
				
	struct stat st;
	stat(resource, &st);
	int fsize = st.st_size;
	
	char *buf = (char *)calloc(fsize, sizeof(char));
	memset(buf, '\0', fsize);
	char c;
	int i = 0;
	
	while ( (c = fgetc(html)) != EOF)
	{
		buf[i] = c;
		i++;
	}
	buf[fsize] = '\0';
	
	//Building response
	char *fsize_s = (char *)calloc(12, sizeof(char));
	sprintf(fsize_s, "%d", fsize);
	
	printf("Size of file is %s bytes", fsize_s);
	
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

	printf("Writing response...");
	if ( write(n, response, strlen(response)) == 0)
	{
		perror("Could not write response to socket");
	}
	else
	{
		printf("OK\n");
	}
	
	free(response);
	free(header);
	free(buf);
	fclose(html);
}

void serveCSS(char *resource, int n)
{
	FILE *css = fopen(resource, "r");
				
	struct stat st;
	stat(resource, &st);
	int fsize = st.st_size;
	
	char *buf = (char *)calloc(fsize, sizeof(char));
	memset(buf, '\0', fsize);
	char c;
	int i = 0;
	
	while ( (c = fgetc(css)) != EOF)
	{
		buf[i] = c;
		i++;
	}
	buf[fsize] = '\0';
	
	//Building response
	char *fsize_s = (char *)calloc(12, sizeof(char));
	sprintf(fsize_s, "%d", fsize);
	
	printf("Size of file is %s bytes", fsize_s);
	
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

	printf("Writing response...");
	if ( write(n, response, strlen(response)) == 0)
	{
		perror("Could not write response to socket");
	}
	else
	{
		printf("OK (wrote %lu bytes)\n", strlen(response));
	}
	
	free(response);
	free(header);
	free(buf);
	fclose(css);
}

void servePHP(char *resource, int n)
{
	//Get PHP involved in this mess
	char* phpcommand = (char *)malloc((strlen("php ") + strlen(resource) + 1) * sizeof(char));
	memset(phpcommand, '\0', sizeof(phpcommand));
	strcat(phpcommand, "php ");
	strcat(phpcommand, resource);
	
	FILE *php;
	if ( (php = (FILE *)popen(phpcommand, "r")) <= 0 )
	{
		//Return 500
		pclose(php);
		printf("PHP failed");
		serveError("500", n);
	}
	else
	{
		unsigned long fsize = 0;
		char *buf = (char *)calloc(UINT_MAX, sizeof(char));
		memset(buf, '\0', UINT_MAX);
		char c;
		
		while ( (c = fgetc(php)) != EOF)
		{
			buf[fsize] = c;
			fsize++;
		}
		buf[fsize] = '\0';
		
		//Building response
		char *fsize_s = (char *)calloc(256, sizeof(char));
		sprintf(fsize_s, "%lu", fsize);
		printf("Size of file is %s bytes", fsize_s);
		
		char *header = (char *)calloc( strlen("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ") + strlen(fsize_s) + strlen("\nConnection: close\n\n") + 1, sizeof(char) );
		memset(header, '\0', sizeof(header));
		strcat(header, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ");
		strcat(header, fsize_s);
		strcat(header, "\nConnection: close\n\n");
		
		printf(", total response size is %lu bytes\n", strlen(header) + fsize);
		
		char *response = (char *)calloc( (strlen(header) + fsize), sizeof(char) );
		memset(response, '\0', sizeof(response) * sizeof(char));
		strcat(response, header);
		strcat(response, buf);
		
		printf("Writing response...");
		if ( write(n, response, strlen(response)) == 0)
		{
			perror("Could not write response to socket");
		}
		else
		{
			printf("OK (wrote %lu bytes)\n", strlen(response));
		}
		
		if (strlen(response) != (strlen(header) + fsize))
			printf("Warning: there might be an issue with response size (%lu vs (%lu + %lu + 1))\n", strlen(response), strlen(header), strlen(buf));
		
		free(response);
		free(header);
		free(buf);
		fclose(php);
	}
	
	free(phpcommand);
}

void respond(int n)
{	
	Message msg;
	msg.content = (char *)malloc(USHRT_MAX * sizeof(char));
	memset(msg.content, '\0', USHRT_MAX * sizeof(char));
	msg.length = recv(n, msg.content, USHRT_MAX * sizeof(char), 0);
	
	if (msg.length < 0)
		printf("recv() error\n");
	else if (msg.length == 0)
		printf("Client disconnected unexpectedly\n");
	else
	{
		printf("Here is what they have to say: \n%s", msg.content);
			
		char *method = strtok(msg.content,  " \t\r\n");
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
			//Return 404
			printf("Resource %s not found, sending 404\n", &uri[1]);
			serveError("404", n);
		}
		else
		{
			printf("Found resource %s\n", &uri[1]);
			
			//Handling snowflake cases
			if (strcmp(&uri[1], "info.php") == 0)
			{
				//getInfo(n);	//Bugged
				serveError("500", n);
			}
			else
			{			
				//Do we need PHP ?
				if (strstr(&uri[1], ".php") != NULL)
				{
					//Do we *have* PHP ?!
					if ( access("/usr/bin/php", F_OK) != -1 )
						servePHP(&uri[1], n);
				}
				else
				{
					if (strstr(&uri[1], ".css") != NULL)
						serveCSS(&uri[1], n);
					else
						serveHTML(&uri[1], n);
				}
			}
		}
	}
	
	free(msg.content);
	close(n);
	printf("Worker has finished !\n");
}