# compiler to use
CC=gcc

# enable debug symbols and warnings
CFLAGS=-c -Wall

all: server client pipe

server: server.o
	$(CC) server.o -o server

client: client.o
	$(CC) client.o -o client

pipe: pipe.o
	$(CC) pipe.o -o pipe

server.o: server.c
	$(CC) $(CFLAGS) server.c

client.o: client.c
	$(CC) $(CFLAGS) client.c

pipe.o: pipe.c
	$(CC) $(CFLAGS) pipe.c

clean:
	rm -rf *.o client server pipe

