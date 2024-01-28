#ifndef MSG_H //grew from holding methods to handle the message struct to general purpose utility file because i 
#define MSG_H //couldnt be bothered to figture out how make works again after making it link this one properly

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

struct message* stringToStruct(char *in);

void getServerInfo(char* input, char* ip, char* name);

void runWho(char* toOut);

void printPay(struct message* msg);

void getClientAddress(int connfd, char* ipOut);

#endif