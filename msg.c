#ifndef MSG_C
#define MSG_C
#include "msg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAXLINE    4096

struct message* init() {
    
    struct message* msg = malloc(sizeof(message));
    msg->addrlen = 0;
    msg->timelen = 0;
    msg->msglen = 0;
    memset(msg->addr, 0, MAXLINE);
    memset(msg->currtime, 0, MAXLINE);
    memset(msg->payload, 0, MAXLINE);
    return msg;
}

void printMsg(struct message* msg) {
    printf("Address Length: %d\n", msg->addrlen);
    printf("Time Length: %d\n", msg->timelen);
    printf("Message Length: %d\n", msg->msglen);
    printf("Address: %s\n", msg->addr);
    printf("Current Time: %s\n", msg->currtime);
    printf("Payload: %s\n", msg->payload);
}

void structToString(struct message *msg, char *result, int size) {
    snprintf(result, size, "%d,%d,%d,%s,%s,%s\n",msg->addrlen, msg->timelen, msg->msglen,msg->addr, msg->currtime, msg->payload);
}

struct message* stringToStruct(char *in){
    struct message* msg = malloc(sizeof(message));
    sscanf(in, "%d,%d,%d,%[^,],%[^,],%[^\n]", &msg->addrlen, &msg->timelen, &msg->msglen, msg->addr, msg->currtime, msg->payload);
    return msg;
}

void getServerInfo(char* input, char* ipOut, char* nameOut) { //takes in the ip or hostname nad returns both
    struct addrinfo hints, *result, *rp;
    struct sockaddr_in *addr;
    char ip[MAXLINE];
    char host[MAXLINE];

    // Initialize hints
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    // Get address information
    if (getaddrinfo(input, NULL, &hints, &result) != 0) {
        fprintf(stderr, "Error in getaddrinfo\n");
        return;
    }

    // Iterate through the results
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        addr = (struct sockaddr_in *)rp->ai_addr;

        // Convert IP address to string
        if (inet_ntop(AF_INET, &(addr->sin_addr), ip, MAXLINE) == NULL) {
            fprintf(stderr, "Error in inet_ntop\n");
            continue;
        }

        if (getnameinfo(rp->ai_addr, rp->ai_addrlen, host, MAXLINE, NULL, 0, 0) == 0) {
            printf("Server Name: %s\n", host);
        }

        // Print the result
        strcpy(ipOut , ip);
        strcpy(nameOut , host);
        printf("\n\n");
        printf("Server Name: %s\n", host);
        printf("Server IP: %s\n", ip);
        printf("\n\n");
    }

    // Free the memory allocated by getaddrinfo
    freeaddrinfo(result);
}


#endif