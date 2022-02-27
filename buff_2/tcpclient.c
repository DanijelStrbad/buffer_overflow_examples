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
	printf("tcpclient [-s server] [-p server port] [-c] [-v] [-h] filename\n c - continue transmission of the file 'filename'\n (not implemented)\n");
	return;
}



int main(int argc, char *argv[]) {
	char debug = (char)(0);
	
	int flagS = 0, flagP = 0, flagC = 0;
	char inChar;
	char fileName[75];
	char cliPort[10] = "2121";
	char portIn[10] = "1234";
	char ipIn[50] = "127.0.0.1";
	int locPort, locIP;
	int i, onFlag = 1;
	int err123;
	
	struct addrinfo hints, *res;
	
	int sockTCP;
	ssize_t recvLen;
	char netBuff[MAXLEN];
	
	FILE *locFile;
	
	
	
	while( (inChar = getopt(argc, argv, "s:p:cvh")) != -1) {
		switch(inChar) {
			case 's':	flagS = 1;
						locIP = optind-1;
						break;
			case 'p':	flagP = 1;
						locPort = optind-1;
						break;
			case 'c':	flagC = 1;
						break;
			case 'v':	debug = 1;
						break;
			default: printHelp();
				err(1, "\n\nWrong input option(s) (exit code 1)\n");
		}
	}
	if( argc-optind != 1) {
		printHelp();
		err(1, "\n\nWrong input option(s) (exit code 1)\n");
	}
	for(i = 0; 1==1; i++) {
		fileName[i] = *(argv[argc-1] + i);
		if( fileName[i] == (char)0 ) {
			break;
		}
	}
	if(debug) {
		printf("\nFile name: %s\n", fileName);
	}
	
	
	
	
	
	/* net */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	//hints.ai_flags |= AI_PASSIVE;
	Getaddrinfo(NULL, cliPort, &hints, &res);
	printf("\nPrvi prosao\n");
	
	sockTCP = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if( setsockopt(sockTCP, SOL_SOCKET, SO_REUSEADDR, &onFlag, sizeof(int)) == -1 ) {
		err(3, "\n\nNet problem - setsockopt (exit code 22)\n");
	}
	
	
	if( (flagS == 0) && (flagP == 0) ) {
		Getaddrinfo(ipIn, portIn, &hints, &res);
		if(debug) {
			printf("\n%s %s\n", ipIn, portIn);
		}
		
	} else if( (flagS == 1) && (flagP == 0) ) {
		Getaddrinfo(argv[locIP], portIn, &hints, &res);
		if(debug) {
			printf("\n%s %s\n", argv[locIP], portIn);
		}
		
	} else if( (flagS == 0) && (flagP == 1) ) {
		Getaddrinfo(ipIn, argv[locPort], &hints, &res);
		if(debug) {
			printf("\n%s %s\n", ipIn, argv[locPort]);
		}
		
	} else {
		Getaddrinfo(argv[locIP], argv[locPort], &hints, &res);
		if(debug) {
			printf("\n%s %s\n", argv[locIP], argv[locPort]);
		}
	}
	
	Connect(sockTCP, res->ai_addr, res->ai_addrlen);
	
	
	/* prepare string for trasfer */
	netBuff[0] = (char) 0;
	netBuff[1] = (char) 0;
	netBuff[2] = (char) 0;
	netBuff[3] = (char) 123;
	for(i = 0; 1; i++) {
		netBuff[i+4] = fileName[i];
		if( netBuff[i+4] == (char)0 ) {
			break;
		}
	}
	
	/* transfer */
	Send(sockTCP, netBuff, ((sizeof (char)) * strlen(fileName)) + 5, 0);
	if(debug) {
		printf("\nMessage sent to server\n");
	}
	
	memset(netBuff, (char) 0, sizeof(netBuff));
	recvLen = Recv(sockTCP, netBuff, 1, 0);
	if( (int)(*netBuff) == 0 ) {
		
		
		/* local file */
		locFile = fopen(fileName, "wb");
		if( locFile == NULL ) {
			err(75, "\n\nFile error (exit code 75)\n");
		}
		
		
		if(debug) {
			printf("\nFirst byte: %d, recvLen = %ld\n", (int)(*netBuff), recvLen);
		}
	} else {
		if(debug) {
			printf("\nFirst byte: %d, recvLen = %ld\nFile error on server side\n", (int)(*netBuff), recvLen);
		}
		close(sockTCP);
		return 3;
	}
	
	while(1) {
		memset(netBuff, (char) 0, sizeof(netBuff));
		recvLen = Recv(sockTCP, netBuff, MAXLEN-1, 0);
		if( recvLen == 0 ) {
			break;
		}
		fwrite(netBuff, sizeof (char), recvLen, locFile);
		if(debug) {
			printf("\nWrite in file: %s\n\n", netBuff);
		}
	}
	
	/* close */
	fclose(locFile);
	close(sockTCP);
	return 0;
}


