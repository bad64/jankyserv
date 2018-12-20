#include "includes.h"

void error(char* msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char const *argv[]) 
{ 
	if (argc != 2)
	{
         printf("ERROR: Please provide a port number to listen on\n");	//If we have no port, we print this and exit
		 exit(1);
    }
	
	//Ugly ASCII banner
	printf("\
NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN\n\
MMy::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::yMM\n\
MMo       ``````      ``     ``    `  ``    `` ``     ``    ````   ```````   ````    ``      ``  oMM\n\
MMo      .mmmMMN-  -sNMM-  /mMN  :mM/+Nn mhNmy.Mm  -odMmdhNmddmm. +NNmmmmd`:mMmmMMo /Mm   :sNN+  oMM\n\
MMo        :mMs`-smN  Md -dMyNM.hMh+mMMNho:`   hMhNms/``mMmho`  :mMMmdd+ .hMMmdNmy- mM+-sNNs:    oMM\n\
MMo  /o//odMh/smNmmmmMM/yMd- NMMd+hMh-hM/    `sMm/. :+///odMN/.hMMsoo:.`sMm/-hM:   /MMmNs:       oMM\n\
MMo  :oso+:` +o:`   `o+-o/   +o/ :o:  `oo`   .o+    /ossso+:` :oooooo. .o+`  :o-   -oo:          oMM\n\
MMo                                                                                        v0.1a oMM\n\
MMy::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::yMM\n\
NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN"\
	);
	printf("\n\n");
	
	chdir("html");

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
		error("Cannot open socket");
	
	//Bind socket
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("Binding failed");
	
	//Now we can listen
	listen(sockfd, 5);
	printf("Listening...\n");
	
	//And when we get a connection...
	clilen = sizeof(cli_addr);
	
	while (1)
	{
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);	//accept() blocks the program until a client connects
		if (newsockfd < 0) 
			printf("Could not accept client connection from %s", inet_ntoa(cli_addr.sin_addr));
		else
		{
			printf("-----------------------\n");
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