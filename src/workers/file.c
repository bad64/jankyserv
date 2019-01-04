#include "../includes.h"

void serveOctetStream(char *resource, int newsockfd)
{
	// Get file info
	FILE *file = fopen(resource, "r");

	struct stat st;
	stat(resource, &st);
	unsigned int fsize = st.st_size;
	
	char *buf = (char *)calloc(fsize, sizeof(char));
	fread(buf, fsize, 1, file);
	fclose(file);
	
	char *fsize_s = (char *)calloc(12, sizeof(char));
	sprintf(fsize_s, "%d", fsize);
	
	printf("Size of file is %u bytes", fsize);
	
	char *header = (char *)calloc( strlen("HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: ") + strlen(fsize_s) + strlen("\r\nConnection: close\r\n\r\n") + 1, sizeof(char) );
	memset(header, '\0', sizeof(header));
	strcat(header, "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: ");
	strcat(header, fsize_s);
	strcat(header, "\r\nConnection: close\r\n\r\n");
	
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
	printf("HTML: My work here is done...\n\n");
}