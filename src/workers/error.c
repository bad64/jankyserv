#include "../includes.h"

void serveError(int errorcode, int newsockfd)
{
	char *error;
	if (errorcode == 403)
	{
		char *body = "<!DOCTYPE html><html style=\"background: black; color: green; text-align: center;\"><head><title>Error</title></head><body><h1>403 Forbidden</h1><hr><p>JankyServ v0.1a</p></body></html>";
		
		char *size_s = (char *)malloc(strlen("65535") * sizeof(char));
		memset(size_s, '\0', sizeof(size_s));							// Basically we're just converting the length of the body to an int
		sprintf(size_s, "%lu", strlen(body));							// so we can pass it to calloc() just below
		
		error = (char *)calloc( strlen("HTTP/1.1 403 Forbidden\nContent-Type: text/html\nContent-Length: ") + strlen(size_s) + strlen("\nConnection: close\n\n") + 1, sizeof(char) );
		memset(error, '\0', sizeof(error));
		strcat(error, "HTTP/1.1 403 Forbidden\nContent-Type: text/html\nContent-Length: ");
		strcat(error, size_s);
		strcat(error, "\nConnection: close\n\n");
		strcat(error, body);
	}
	else if (errorcode == 404)
	{
		char *body = "<!DOCTYPE html><html style=\"background: black; color: green; text-align: center;\"><head><title>Error</title></head><body><h1>404 Not Found</h1><hr><p>JankyServ v0.1a</p></body></html>";
		
		char *size_s = (char *)malloc(strlen("65535") * sizeof(char));
		memset(size_s, '\0', sizeof(size_s));
		sprintf(size_s, "%lu", strlen(body));
		
		error = (char *)calloc( strlen("HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: ") + strlen(size_s) + strlen("\nConnection: close\n\n") + 1, sizeof(char) );
		memset(error, '\0', sizeof(error));
		strcat(error, "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: ");
		strcat(error, size_s);
		strcat(error, "\nConnection: close\n\n");
		strcat(error, body);
	}
	else if (errorcode == 501)
	{
		char *body = "<!DOCTYPE html><html style=\"background: black; color: green; text-align: center;\"><head><title>Error</title></head><body><h1>501 Not Implemented</h1><hr><p>JankyServ v0.1a</p></body></html>";
		
		char *size_s = (char *)malloc(strlen("65535") * sizeof(char));
		memset(size_s, '\0', sizeof(size_s));
		sprintf(size_s, "%lu", strlen(body));
		
		error = (char *)calloc( strlen("HTTP/1.1 501 Not Implemented\nContent-Type: text/html\nContent-Length: ") + strlen(size_s) + strlen("\nConnection: close\n\n") + 1, sizeof(char) );
		memset(error, '\0', sizeof(error));
		strcat(error, "HTTP/1.1 501 Not implemented\nContent-Type: text/html\nContent-Length: ");
		strcat(error, size_s);
		strcat(error, "\nConnection: close\n\n");
		strcat(error, body);
	}
	else	//Generic error
	{
		char *body = "<!DOCTYPE html><html style=\"background: black; color: green; text-align: center;\"><head><title>Error</title></head><body><h1>500 Internal Server Error</h1><hr><p>JankyServ v0.1a</p></body></html>";
		
		char *size_s = (char *)malloc(strlen("65535") * sizeof(char));
		memset(size_s, '\0', sizeof(size_s));
		sprintf(size_s, "%lu", strlen(body));
		
		error = (char *)calloc( strlen("HTTP/1.1 500 Internal Server Error\nContent-Type: text/html\nContent-Length: ") + strlen(size_s) + strlen("\nConnection: close\n\n") + 1, sizeof(char) );
		memset(error, '\0', sizeof(error));
		strcat(error, "HTTP/1.1 500 Internal Server Error\nContent-Type: text/html\nContent-Length: ");
		strcat(error, size_s);
		strcat(error, "\nConnection: close\n\n");
		strcat(error, body);
	}
	
	printf("Writing response... ");
	if ( write(newsockfd, error, strlen(error)) == 0)
	{
		perror("Could not write response to socket");
	}
	else
	{
		printf("OK\n");
	}
	
	printf("Error worker has finished !\n");
}