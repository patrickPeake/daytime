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
            //printMsg(msg);

            printf("Pipe recieved request to contact server %s on port %s", msg->addr, msg->payload);

            //printMsg(msg); //print the message to be sent

            //TODO: stick a client in the server loop here


            int     sockfd, n;
            char    recvline[MAXLINE + 1];
            struct sockaddr_in servaddr;


            if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                printf("socket error\n");
                exit(1);
            }

            char argin[MAXLINE]; 
            strcpy(argin , msg->addr);
            char* ip = malloc(MAXLINE);
            char* name = malloc(MAXLINE);
            char recon[MAXLINE*8] = "";


            getServerInfo(argin, ip, name); //pass in either hostname or ip, get both back out

            bzero(&servaddr, sizeof(servaddr));
            servaddr.sin_family = AF_INET; 
            servaddr.sin_port = htons(atoi(msg->payload));  /* daytime server */
            if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0) {
                printf("inet_pton error for %s\n", ip);
                exit(1);
            }
            
            char *message = malloc(MAXLINE*4); 
            structToString(msg, message, MAXLINE*4+1); // format message to be sent to server

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
            }

            free(msg); //free the old struct before assigning out to a new one
            msg = stringToStruct(recon); //get new struct with correct fields from the server
            //printf("Server Name: %s\nIP Address: %s\n\n", name, ip);
            //printPay(out);


            if (n < 0) {
                printf("read error\n");
                exit(1);
            }

            //free(msg); //free stuff
            free(ip);
            free(name);
            //free(message);




            /////////////////////////////////////////////////////





            structToString(msg, message, MAXLINE*4+1); //serialize the struct

            write(connfd, message, strlen(message)); //send it out
            free(message);
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
