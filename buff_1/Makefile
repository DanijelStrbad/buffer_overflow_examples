#CC = gcc

CFLAGS = -Wall -g
LDFLAGS = 

all: main

main: main.o
	cc -o main main.o

main.o: main.c
	cc -c main.c


clean:
	-rm -f main *.o *.core

