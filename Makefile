CC=gcc

CFLAGS=-c -g -Wall

all: mysh

mysh: mysh.o
	$(CC) mysh.o -o mysh

mysh.o: mysh.c
	$(CC) $(CFLAGS) mysh.c

clean:
	rm -rf *.o *.txt mysh
