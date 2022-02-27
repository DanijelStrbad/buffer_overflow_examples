#CC = gcc

CFLAGS = -Wall -g
LDFLAGS = 

all: tcpclient tcpserver

tcpclient: tcpclient.o net_functions.o
		cc -o tcpclient tcpclient.o net_functions.o

tcpclient.o: tcpclient.c net_functions.h
		cc -c tcpclient.c

tcpserver: tcpserver.o net_functions.o
		cc -o tcpserver tcpserver.o net_functions.o

tcpserver.o: tcpserver.c net_functions.h
		cc -c tcpserver.c

net_functions.o: net_functions.c net_functions.h
		cc -c net_functions.c


clean:
		-rm -f tcpclient tcpserver *.o *.core

