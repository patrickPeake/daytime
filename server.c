#include <netinet/in.h>
#include <time.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> 
#include "msg.c"

#define MAXLINE     4096    /* max text line length */
#define LISTENQ     1024    /* 2nd argument to listen() */
#define DAYTIME_PORT 3333

int
main(int argc, char **argv)
{   
    if (argc != 2) {
        printf("usage: server <port>\n");
        exit(1);
    }
    int     listenfd, connfd;
    struct sockaddr_in servaddr;
    char    buff[MAXLINE];
    time_t ticks;
    struct message* msg;
    char* inptStr = malloc(MAXLINE);
    char* ip = malloc(MAXLINE);
    char* name = malloc(MAXLINE);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1])); /* daytime server */

    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    for ( ; ; ) {
        connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);

        getClientAddress(connfd, inptStr);
        getServerInfo(inptStr, ip, name);
        printf("Client Name: %s\nIP Address: %s\n", name, ip);
        
        char buffin[MAXLINE*4+1];
        int ret = read(connfd, buffin, 4*MAXLINE);
        if (ret > 0) {
            buffin[ret] = '\0';  // Null-terminate the received data

            msg = stringToStruct(buffin); //create a struct with the info from the client
            printMsg(msg);
            runWho(buff); //put the output of who into buff

            strcpy(msg->payload, buff); //put that in the payload and set the length
            msg->msglen = strlen(msg->payload);

            ticks = time(NULL);
            sprintf(msg->currtime, "%.24s", ctime(&ticks)); //repopulate the time
            msg->timelen = strlen(msg->currtime);

            strcpy(msg->addr, ip);
            msg->addrlen = strlen(msg->addr); //and IP fields

            //printMsg(msg); //print the message to be sent

            structToString(msg, buffin, MAXLINE*4+1); //serialize the struct

            write(connfd, buffin, strlen(buffin)); //send it out
        } else { //souldnt ever really happen but will prevent client and server getting stuck if a bodyless message comes through
            ticks = time(NULL);
            snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
            write(connfd, buff, strlen(buff));
            printf("Sending response: %s", buff);
        }

        free(msg); //free stuff
        
        close(connfd);
    }
    free(inptStr);
    free(ip);
    free(name);
}
