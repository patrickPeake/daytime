#include "msg.h"
#include "stdio.h"
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
    printf("%d,%d,%d,%s,%s,%s\n",msg->addrlen, msg->timelen, msg->msglen,msg->addr, msg->currtime, msg->payload);
    //printf("snprintf return: %d", snprintf(result, size, "%d,%d,%d,%s,%s,%s\n",msg->addrlen, msg->timelen, msg->msglen,msg->addr, msg->currtime, msg->payload));
    printf("\n\n\n\n\n");
}