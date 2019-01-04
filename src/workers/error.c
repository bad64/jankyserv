#include "../includes.h"

void serveError(int errorcode, int newsockfd)
{
	char *error;
	if (errorcode == 403)
	{
		char *body = "<!DOCTYPE html><html style=\"background: black; color: green; text-align: center;\"><head><title>Error</title></head><body><h1>403 Forbidden</h1><hr><p>JankyServ v0.11a</p></body></html>";
		
		char *size_s = (char *)malloc(strlen("65535") * sizeof(char));
		memset(size_s, '\0', sizeof(size_s));							// Basically we're just converting the length of the body to an int
		sprintf(size_s, "%lu", strlen(body));							// so we can pass it to calloc() just below
		
		error = (char *)calloc( strlen("HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\nContent-Length: ") + strlen(size_s) + strlen("\r\nConnection: close\r\n\r\n") + 1, sizeof(char) );
		memset(error, '\0', sizeof(error));
		strcat(error, "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\nContent-Length: ");
		strcat(error, size_s);
		strcat(error, "\r\nConnection: close\r\n\r\n");
		strcat(error, body);
		
		free(size_s);
	}
	else if (errorcode == 404)
	{
		char *body = "<!DOCTYPE html><html style=\"background: black; color: green; text-align: center;\"><head><title>Error</title></head><body><h1>404 Not Found</h1><hr><p>JankyServ v0.11a</p></body></html>";
		
		char *size_s = (char *)malloc(strlen("65535") * sizeof(char));
		memset(size_s, '\0', sizeof(size_s));
		sprintf(size_s, "%lu", strlen(body));
		
		error = (char *)calloc( strlen("HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: ") + strlen(size_s) + strlen("\r\nConnection: close\r\n\r\n") + 1, sizeof(char) );
		memset(error, '\0', sizeof(error));
		strcat(error, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: ");
		strcat(error, size_s);
		strcat(error, "\r\nConnection: close\r\n\r\n");
		strcat(error, body);
		
		free(size_s);
	}
	else if (errorcode == 418)
	{
		char *body = "<!DOCTYPE html><html style=\"background: black; color: green; text-align: center;\"><head><title>Error</title></head><body><h1>418 I'm A Teapot</h1><hr><p>JankyServ v0.11a</p></body></html>";
		
		char *size_s = (char *)malloc(strlen("65535") * sizeof(char));
		memset(size_s, '\0', sizeof(size_s));
		sprintf(size_s, "%lu", strlen(body));
		
		error = (char *)calloc( strlen("HTTP/1.1 418 I'm A Teapot\r\nContent-Type: text/html\r\nContent-Length: ") + strlen(size_s) + strlen("\r\nConnection: close\r\n\r\n") + 1, sizeof(char) );
		memset(error, '\0', sizeof(error));
		strcat(error, "HTTP/1.1 418 I'm A Teapot\r\nContent-Type: text/html\r\nContent-Length: ");
		strcat(error, size_s);
		strcat(error, "\r\nConnection: close\r\n\r\n");
		strcat(error, body);
		
		free(size_s);
	}
	else if (errorcode == 501)
	{
		char *body = "<!DOCTYPE html><html style=\"background: black; color: green; text-align: center;\"><head><title>Error</title></head><body><h1>501 Not Implemented</h1><hr><p>JankyServ v0.11a</p></body></html>";
		
		char *size_s = (char *)malloc(strlen("65535") * sizeof(char));
		memset(size_s, '\0', sizeof(size_s));
		sprintf(size_s, "%lu", strlen(body));
		
		error = (char *)calloc( strlen("HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\nContent-Length: ") + strlen(size_s) + strlen("\r\nConnection: close\r\n\r\n") + 1, sizeof(char) );
		memset(error, '\0', sizeof(error));
		strcat(error, "HTTP/1.1 501 Not implemented\r\nContent-Type: text/html\r\nContent-Length: ");
		strcat(error, size_s);
		strcat(error, "\r\nConnection: close\r\n\r\n");
		strcat(error, body);
		
		free(size_s);
	}
	else	//Generic error
	{
		char *body = "<!DOCTYPE html><html style=\"background: black; color: green; text-align: center;\"><head><title>Error</title></head><body><h1>500 Internal Server Error</h1><hr><p>JankyServ v0.11a</p></body></html>";
		
		char *size_s = (char *)malloc(strlen("65535") * sizeof(char));
		memset(size_s, '\0', sizeof(size_s));
		sprintf(size_s, "%lu", strlen(body));
		
		error = (char *)calloc( strlen("HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: ") + strlen(size_s) + strlen("\r\nConnection: close\r\n\r\n") + 1, sizeof(char) );
		memset(error, '\0', sizeof(error));
		strcat(error, "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: ");
		strcat(error, size_s);
		strcat(error, "\r\nConnection: close\r\n\r\n");
		strcat(error, body);
		
		free(size_s);
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
	
	free(error);
	printf("Error worker has finished !\n\n");	// Yes, it differs from the standard end of work message so people don't think it is an error message due to the name of the worker
}