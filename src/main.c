#include "includes.h"

int main(int argc, char const *argv[]) 
{
	//Check if args are ok
	if (argc != 2)
	{
         printf("ERROR: Please provide a port number to listen on\n");	//If we have no port, we print this and exit
		 exit(1);
    }
	
	//Check if a base directory has been configured during compilation
	#ifndef BASEDIR
		#define BASEDIR .
	#endif
	
	//Change working directory to basedir if it is not the current working directory
	char *basedir = STR(BASEDIR);
	
	if (strcmp(basedir, ".") == 0)
	{
		if (chdir("html") != 0)
		{
			switch (errno)
			{
				case ENOENT:
					if (mkdir("html", 0755) != 0)
					{
						perror("Cannot create html directory");
						exit(1);
					}
					else
					{
						printf("Created html directory\n");
						chdir("html");
					}
					break;
				case ENOTDIR:
					printf("A file named \"html\" is present in the server directory. The server cannot run without a html folder to pull pages from. Exiting.\n");
					exit(1);
					break;
				case EACCES:
					perror("Could not access html folder");
					exit(1);
					break;
			}
		}
	}
	else
	{
		if (chdir(basedir) != 0)
		{
			switch (errno)
			{
				char *errmsg;
				case ENOENT:
					if (mkdir(basedir, 0755) != 0)
					{
						errmsg = (char *)malloc( (strlen("Cannot create server directory at ") + strlen(basedir)) * sizeof(char));
						memset(errmsg, '\0', sizeof(errmsg));
						strcat(errmsg, "Cannot create server directory at ");
						strcat(errmsg, basedir);
						
						perror(errmsg);
						exit(1);
					}
					else
					{
						printf("Created server directory at %s\n", basedir);
						chdir(basedir);
					}
					break;
				case ENOTDIR:
					printf("%s is not a folder. The server cannot start without a folder to pull pages from. Exiting.\n", basedir);
					exit(1);
					break;
				case EACCES:
					errmsg = (char *)malloc( (strlen("Could not access directory at ") + strlen(basedir)) * sizeof(char));
					memset(errmsg, '\0', sizeof(errmsg));
					strcat(errmsg, "Could not access directory at ");
					strcat(errmsg, basedir);
					
					perror(errmsg);
					exit(1);
					break;
			}
		}
	}
	
	//Ugly ASCII banner
	printf("╔═══════════════════════════════════════════════════════════════════════════════════╗\n");
	printf("║                                                                                   ║\n");
	printf("║   ██████████ ████████ ███    ██ ██  ██ ██    ██ ██████ ██████ ██████   ██    ██   ║\n");
	printf("║           ██ ██    ██ ████   ██ ██ ██   ██  ██  ██     ██     ██    ██ ██    ██   ║\n");
	printf("║           ██ ██    ██ ██ ██  ██ ████     ████   ██     ██     ██    ██  ██  ██    ║\n");
	printf("║          ███ █████ ██ ██  ██ ██ ███       ██    ██████ ████   ██████    ██  ██    ║\n");
	printf("║          ███ ██    ██ ██   ████ ████      ██        ██ ██     ██  ██     ████     ║\n");
	printf("║   ██    ████ ██    ██ ██    ███ ██ ██     ██        ██ ██     ██   ██    ████     ║\n");
	printf("║   ██████████ ██    ██ ██     ██ ██  ██    ██    ██████ ██████ ██    ██    ██      ║\n");
	printf("║                                                                                   ║\n");
	printf("╚═══════════════════════════════════════════════════════════════════════════════════╝\n");
	printf("\n");

	
	//Actually starting the server stuff
    int sockfd, newsockfd;									//File descriptors for sockets 
	int port = (int)strtol(argv[1], 0, 10);					//Self explanatory, passed via args
	int clilen;												//Length of client address. Required by the accept() system call
	
	struct sockaddr_in serv_addr, cli_addr;					//Also relatively self explanatory
	memset(&serv_addr, '\0', sizeof(serv_addr));			//Zero the server address. memset() is standard, unlike bzero()

	serv_addr.sin_family = AF_INET;							//Pretty much a constant, or so I understand
	serv_addr.sin_addr.s_addr = INADDR_ANY;					//Also a constant that (usually) holds the host IP
	serv_addr.sin_port = htons(port);						//htons() converts the unsigned short integer hostshort from host byte order to network byte order
	
	//Open socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	{
		perror("Cannot open socket");
		exit(1);
	}
	
	//Bind socket
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	{
		perror("Binding failed");
	}
	
	//Now we can listen
	listen(sockfd, 5);
	printf("Listening on port %s...\n", argv[1]);
	
	//And when we get a connection...
	clilen = sizeof(cli_addr);
	
	while (1)
	{
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);	//accept() blocks the program until a client connects
		if (newsockfd < 0) 
			printf("Could not accept client connection from %s", inet_ntoa(cli_addr.sin_addr));
		else
		{
			printf("=========================\n");
			printf("Connection accepted from %s\n", inet_ntoa(cli_addr.sin_addr));

			if ( fork() == 0)
			{
				respond(newsockfd);
				
				if ( write(newsockfd, '\0', 1) != -1)
					printf("WARNING: Socket did not close properly\n");
				
				exit(0);
			}
		}
	}
	
	close(sockfd);	//Close server socket before terminating program
	
	return 0;
}