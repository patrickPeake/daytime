#ifndef MSG_H
#define MSG_H

#define MAXLINE     4096    /* max text line length */


typedef struct message{
    int addrlen, timelen, msglen;
    char addr[MAXLINE];
    char currtime[MAXLINE];
    char payload[MAXLINE];
} message;

struct message* init();

void printMsg(struct message* msg);

void structToString(struct message *msg, char *result, int size);


#endif