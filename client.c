#include <netinet/in.h> //When I tried to implement server addressing by name I was unable to get a real name from my server so I could not address it by name
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "msg.c"
#include <time.h>

#define MAXLINE     4096    /* max text line length */
#define DAYTIME_PORT 3333

int
main(int argc, char **argv)
{
    int     sockfd, n;
    char    recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;

    if (argc != 3) {
        printf("usage: client <IPaddress>, <port>\n");
        exit(1);
    }

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket error\n");
        exit(1);
    }
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));  /* daytime server */
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        printf("inet_pton error for %s\n", argv[1]);
        exit(1);
    }

    time_t ticks;
    struct message out = init();
    snprintf(out.addr, MAXLINE, "%s", argv[1]);
    out.addrlen = strlen(out.addr);
    ticks = time(NULL);
    snprintf(out.currtime, MAXLINE, "%.24s\r\n", ctime(&ticks));
    out.timelen = strlen(out.currtime);
    snprintf(out.payload, MAXLINE, "%s", "Test Output");
    out.msglen = strlen(out.payload);
    printMsg(out);

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        printf("connect error\n");
        exit(1);
    }


/*
    // Send "message sent" to the server
    const char *message = "message sent";
    if (write(sockfd, message, strlen(message)) < 0) {
        printf("write error\n");
        exit(1);
    }*/


    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;        /* null terminate */
        if (fputs(recvline, stdout) == EOF) {
            printf("fputs error\n");
            exit(1);
        }
    }
    if (n < 0) {
        printf("read error\n");
        exit(1);
    }

    exit(0);
}

