#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <signal.h>

typedef struct Message Message;
struct Message
{
	int length;
	char *content;
};

void getInfo(int n);

void serveError(char *errorcode, int n);
void serveHTML(char *resource, int n);
void serveCSS(char *resource, int n);
void servePHP(char *resource, int n);
void respond(int n);