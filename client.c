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
#include <netdb.h>

#define MAXLINE     4096    /* max text line length */
#define DAYTIME_PORT 3333

int
main(int argc, char **argv)
{
    //setbuf(stdout, NULL);//no buffering for stdout
    char argin[MAXLINE]; 
    strcpy(argin , argv[1]);
    int     sockfd, n;
    char    recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;

    if (argc != 3) { //check right number of args
        printf("usage: client <IPaddress>, <port>\n");
        exit(1);
    }


    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket error\n");
        exit(1);
    }

    char* ip = malloc(MAXLINE);
    char* name = malloc(MAXLINE);
    char recon[MAXLINE*8] = "";


    getServerInfo(argin, ip, name);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(atoi(argv[2]));  /* daytime server */
    if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0) {
        printf("inet_pton error for %s\n", ip);
        exit(1);
    }

    time_t ticks; //construct struct
    struct message* out = init();
    snprintf(out->addr, MAXLINE-1, "%s", ip);
    out->addrlen = strlen(out->addr);
    ticks = time(NULL); 
    snprintf(out->currtime, MAXLINE-1, "%.24s", ctime(&ticks));
    out->timelen = strlen(out->currtime);
    snprintf(out->payload, MAXLINE-1, "%s", "Test Output\n");
    out->msglen = strlen(out->payload);
    //printMsg(out); //print struct for debug
    
    // format message to be sent to server
    
    char *message = malloc(MAXLINE*4);
    structToString(out, message, MAXLINE*4+1); 
    //printf("sending: %s\n", message);

 

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        printf("connect error\n");
        exit(1);
    }

    if (write(sockfd, message, strlen(message)) < 0) {
        printf("write error\n");
        exit(1); 
    }


    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;        /* null terminate */
        strcat(recon, recvline); //concatonate the lines together
        //printf("%s", recvline);
        //out = stringToStruct(recvline);
        //printMsg(out);
        /*
        if (fputs(recvline, stdout) == EOF) {
            printf("fputs error\n");
            exit(1);
        }*/
    }

    //printf("%s", recon);

    out = stringToStruct(recon);
    printMsg(out);


    if (n < 0) {
        printf("read error\n");
        exit(1);
    }
    free(out);
    free(ip);
    free(name);
    exit(0);
}

