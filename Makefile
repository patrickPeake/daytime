# compiler to use
CC=gcc

# enable debug symbols and warnings
CFLAGS=-c -Wall

all: server client pipe

server: server.o msg.0
	$(CC) server.o msg.o -o server

client: client.o msg.o
	$(CC) client.o msg.o -o client

pipe: pipe.o msg.o
	$(CC) pipe.o msg.o -o pipe

server.o: server.c
	$(CC) $(CFLAGS) server.c

client.o: client.c
	$(CC) $(CFLAGS) client.c

pipe.o: pipe.c
	$(CC) $(CFLAGS) pipe.c

clean:
	rm -rf *.o client server pipe

