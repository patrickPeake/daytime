#define MAXLINE     4096    /* max text line length */

typedef struct message{
    int addrlen, timelen, msglen;
    char addr[MAXLINE];
    char currtime[MAXLINE];
    char payload[MAXLINE];
} message;