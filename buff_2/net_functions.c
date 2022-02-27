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

void printTerminalOptions(int argc, char *argv[]) {
	// # ./prog abc 123 -t -n -h
	// argv[0] = ./prog
	// argv[1] = abc
	// argv[2] = 123
	// argv[3] = -t
	// argv[4] = -n
	// argv[5] = -h
	int i, j;
	printf("\n=== printTerminalOptions(...) =====\n");
	for(i=0; i<argc; i++) {
		printf("argv[%d] = %s\n", i, argv[i]);
	}
	printf("===================================\n\n");
	return;
}


int Getaddrinfo(const char *restrict node, const char *restrict service, const struct addrinfo *restrict hints, struct addrinfo **restrict res) {
	int ret;
	ret = getaddrinfo(node, service, hints, res);
	if(ret) {
		err(100, "\n\nNet problem - getaddrinfo %d (exit code 100)\n", ret);
	}
	return ret;
}


int Socket(int family, int type, int proto) {
	int sock1;
	sock1 = socket(family, type, proto);
	if(sock1 < 0) {
		err(101, "\n\nNet problem - socket (exit code 101)\n");
	}
	return sock1;
}

int Bind(int sockfd, const struct sockaddr *myaddr, int addrlen) {
	int bind1;
	bind1 = bind(sockfd, myaddr, addrlen);
	if(bind1 < 0) {
		err(102, "\n\nNet problem - bind (exit code 102)\n");
	}
	return bind1;
}


/* TCP */
int Listen(int sockfd, int backlog) {
	int listen1;
	listen1 = listen(sockfd, backlog);
	if(listen1 < 0) {
		err(103, "\n\nNet problem - listen (exit code 103)\n");
	}
	return listen1;
}

int Accept(int sockfd, struct sockaddr* cliaddr, socklen_t *addrlen) {
	int accept1;
	accept1 = accept(sockfd, cliaddr, addrlen);
	if(accept1 < 0) {
		err(104, "\n\nNet problem - accept (exit code 104)\n");
	}
	return accept1;
}

int Connect(int sockfd, const struct sockaddr *server, socklen_t addrlen) {
	int connect1;
	connect1 = connect(sockfd, server, addrlen);
	if(connect1 < 0) {
		err(105, "\n\nNet problem - connect (exit code 105)\n");
	}
	return connect1;
}

ssize_t Send(int sockfd, const void *buf, size_t len, int flags) {
	ssize_t send1;
	send1 = send(sockfd, buf, len, flags);
	if(send1<0) {
		err(106, "\n\nNet problem - send (exit code 106)\n");
	}
	return send1;
}

ssize_t Recv(int sockfd, void *buf, size_t len, int flags) {
	ssize_t recv1;
	recv1 = recv(sockfd, buf, len, flags);
	if(recv1<0) {
		err(107, "\n\nNet problem - recv (exit code 107)\n");
	}
	return recv1;
}

