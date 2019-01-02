#include "../includes.h"

void serveIMG(char *resource, char *ext, int n)
{	
	// Get file info
	FILE *file = fopen(resource, "rb");

	struct stat st;
	stat(resource, &st);
	unsigned int fsize = st.st_size;
	
	char *buf = (char *)calloc(fsize, sizeof(char));
	fread(buf, fsize, 1, file);
	fclose(file);
	
	char *fsize_s = (char *)calloc(12, sizeof(char));
	sprintf(fsize_s, "%d", fsize);
	
	printf("Size of file is %d bytes", fsize);
	
	// Building response
	char *header;
	if ( (strcmp(ext, ".jpeg") == 0) || (strcmp(ext, ".jpg") == 0) )
	{
		header = (char *)calloc( strlen("HTTP/1.1 200 OK\nContent-Type: image/jpeg\nContent-Length: ") + strlen(fsize_s) + strlen("\nConnection: close\n\n") + 1, sizeof(char) );
		memset(header, '\0', sizeof(header));
		strcat(header, "HTTP/1.1 200 OK\nContent-Type: image/jpeg\nContent-Length: ");
	}
	else if (strcmp(ext, ".png") == 0)
	{
		header = (char *)calloc( strlen("HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Length: ") + strlen(fsize_s) + strlen("\nConnection: close\n\n") + 1, sizeof(char) );
		memset(header, '\0', sizeof(header));
		strcat(header, "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Length: ");
	}
	else if (strcmp(ext, ".gif") == 0)
	{
		header = (char *)calloc( strlen("HTTP/1.1 200 OK\nContent-Type: image/gif\nContent-Length: ") + strlen(fsize_s) + strlen("\nConnection: close\n\n") + 1, sizeof(char) );
		memset(header, '\0', sizeof(header));
		strcat(header, "HTTP/1.1 200 OK\nContent-Type: image/gif\nContent-Length: ");
	}
	else if (strcmp(ext, ".tiff") == 0)
	{
		header = (char *)calloc( strlen("HTTP/1.1 200 OK\nContent-Type: image/tiff\nContent-Length: ") + strlen(fsize_s) + strlen("\nConnection: close\n\n") + 1, sizeof(char) );
		memset(header, '\0', sizeof(header));
		strcat(header, "HTTP/1.1 200 OK\nContent-Type: image/tiff\nContent-Length: ");
	}
	
	strcat(header, fsize_s);
	strcat(header, "\nConnection: close\n\n");
	
	printf(", total response size is %lu bytes (%lu bytes header)\n", strlen(header) + fsize, strlen(header));
	
	char *response = (char *)calloc( (strlen(header) + fsize + 1), sizeof(char) );
	memset(response, '\0', sizeof(response) * sizeof(char));
	strcat(response, header);
	memcpy(response + strlen(header), buf, fsize);

	// TODO: Response size error
	printf("Writing response... ");
	if ( write(n, response, strlen(response)) == 0)
	{
		perror("Could not write response to socket");
	}
	else
	{
		printf("OK (wrote %lu bytes)\n", strlen(response));
	}
	
	if (strlen(response) != (strlen(header) + fsize))
	{
		printf("Warning: there might be an issue with response size (%lu vs (%lu + %du + 1))\n", strlen(response), strlen(header), fsize);
	}
	
	free(response);
	free(header);
	free(buf);
	
	printf("IMG: My work here is done...\n");
}