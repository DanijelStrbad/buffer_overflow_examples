#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "net_functions.h"

#define MAXLEN 1400



void printHelp() {
	printf("\nHelp:\n");
	printf("tcpserver [-p port] [-v] [-h]\n\n");
	return;
}



int main(int argc, char *argv[]) {
	char debug = (char)(0);
	
	// char msgBuff[MAXLEN], fileName[100] = "file123.txt";		/* NO bug */
	char msgBuff[4], fileName[100] = "file123.txt";				/* Bug */

	int flagP = 0;
	char inChar;
	char portIn[10] = "1234";
	int i, onFlag = 1;
	
	int sockTCP, newSock;
	struct sockaddr_in addrTCPserv;
	struct addrinfo hints, *res;
	
	struct sockaddr *addrCli;
	socklen_t *cliLen;
	
	ssize_t recvLen;
	char cliId[4];
	int cliIdInt = 0;
	int err123;
	char messageFile[25] = "File error\n";
	
	FILE *locFile;
	char fileBuff[MAXLEN];
	int fileBuffSize;
	
	
	while( (inChar = getopt(argc, argv, "p:v")) != -1) {
		switch(inChar) {
			case 'p':	flagP = 1;
						break;
			case 'v':	debug = 1;
						break;
			default: printHelp();
				err(1, "\n\nWrong input option(s) (exit code 1)\n");
		}
	}
	if( argc-optind != 0) {
		printHelp();
		err(1, "\n\nWrong input option(s) (exit code 1)\n");
	}
	

	/* buffer pointers */
	printf("&msgBuff[0]  = %p\n", (void *) msgBuff);
	printf("&fileName[0] = %p\n", (void *) fileName);


	/* set network */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags |= AI_PASSIVE;
	if(flagP == 0) {
		Getaddrinfo(NULL, portIn, &hints, &res);
		if(debug) {
			printf("\nPort: %s\n", portIn);
		}
	} else {
		Getaddrinfo(NULL, optarg, &hints, &res);
		if(debug) {
			printf("\nPort: %s\n", optarg);
		}
	}
	
	sockTCP = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	if( setsockopt(sockTCP, SOL_SOCKET, SO_REUSEADDR, &onFlag, sizeof(int)) == -1 ) {
		err(22, "\n\nNet problem - setsockopt (exit code 22)\n");
	}
	
	Bind(sockTCP, res->ai_addr, res->ai_addrlen);
	
	Listen(sockTCP, 1);
	if(debug) {
		printf("\nListen done\n");
	}
	
	while(1) {
		newSock = Accept(sockTCP, NULL, NULL);
		if(debug) {
			printf("\nAccept done\n");
			printf("\nfileName = %s\n", fileName);
		}
		
		
		memset(msgBuff, (char) 0, sizeof(msgBuff));
		recvLen = Recv(newSock, msgBuff, MAXLEN, 0);			/* buffer overflow happens here */


		cliId[0] = msgBuff[0];
		cliId[1] = msgBuff[1];
		cliId[2] = msgBuff[2];
		cliId[3] = msgBuff[3]; 
		
		
		cliIdInt = (cliId[0] << 24) | ((cliId[1] & 0xFF) << 16) | 
							((cliId[2] & 0xFF) << 8) | (cliId[3] & 0xFF);
		
		if(debug) {
			printf("\nRecieved:\n");
			for(i = 0; i<recvLen; i++) {
				printf("%c", msgBuff[i]);
			}
			printf("\ncliIdInt = %d\nfileName = %s\n\n", cliIdInt, fileName);
		}
		
		
		locFile = fopen(fileName, "rb");
		if( locFile == NULL ) {
			if(debug) {
				printf("\nFile %s error\n", fileName);
			}
			
			memset(fileBuff, (char) 1, sizeof(fileBuff));
			Send(newSock, fileBuff, 1, 0);
			Send(newSock, messageFile, (sizeof (char)) * strlen(messageFile), 0);
			
		} else {
			if(debug) {
				printf("\nFile %s success\n", fileName);
			}
			
			memset(fileBuff, (char) 0, sizeof(fileBuff));
			Send(newSock, fileBuff, 1, 0);
			
			while( !feof(locFile) ) {
				memset(fileBuff, '\0', sizeof(fileBuff));
				fileBuffSize = fread (fileBuff, sizeof (char), MAXLEN, locFile);
				if(fileBuffSize < 0) {
					err(200, "\n\nFile read problem (exit code 200)\n");
				}
				
				Send(newSock, fileBuff, (sizeof (char))*fileBuffSize, 0);
			}
			if(debug) {
				printf("\nFile %s sent\n", fileName);
			}
		}
		close(newSock);
	}
	
	return 0;
}

