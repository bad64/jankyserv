#include "../includes.h"

void serveHTML(char *resource, int n)
{
	printf("1 ");
	//Get file info
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
		printf("Warning: there might be an issue with response size (%lu vs (%lu + %d))\n", strlen(response), strlen(header), fsize);
	
	free(response);
	free(header);
	free(buf);
}

void serveCSS(char *resource, int n)
{
	//Get file info
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
	
	//Building response	
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
	
	if (strlen(response) != (strlen(header) + fsize))
		printf("Warning: there might be an issue with response size (%lu vs (%lu + %lu + 1))\n", strlen(response), strlen(header), strlen(buf));
	
	free(response);
	free(header);
	free(buf);
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
		serveError(500, n);
	}
	else
	{
		unsigned long fsize = 0;
		char *buf = (char *)malloc(2000 * sizeof(char));
		memset(buf, '\0', 2000);
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