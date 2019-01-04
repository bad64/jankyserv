#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//Compile-time options
#ifndef USE_PHP
#define USE_PHP 1
#endif

#ifdef USE_ZLIB
#include <zlib.h>
#endif

#define EXPAND(tok) #tok
#define STR(tok) EXPAND(tok)

// misc.c
int isElementOf(char *element, char **array, int length);

// respond.c
void respond(int newsockfd);

// workers/text.c
void serveHTML(char *resource, int newsockfd);
void serveCSS(char *resource, int newsockfd);
void servePHP(char *resource, char* clientmessage, char *method, char *args, int newsockfd);

// workers/images.c
void serveIMG(char *resource, char *ext, int newsockfd);

// workers/json.c
// char *serveJSON(char *resource, int newsockfd);

// workers/error.c
void serveError(int errorcode, int newsockfd);

// extras/log.c
// void logTransaction();