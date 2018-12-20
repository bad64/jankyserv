#include "includes.h"

void getInfo(int n)
{
	printf("Getting info...\n");
	char* phpcommand = (char *)malloc((strlen("php-cgi info.php") + 1) * sizeof(char));
	memset(phpcommand, '\0', sizeof(phpcommand));
	strcat(phpcommand, "php-cgi info.php");
	
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