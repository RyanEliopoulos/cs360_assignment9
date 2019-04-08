#define MY_PORT_NUMBER 49999

/* included because the slides suggest doing so */
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
///////////////
#include<string.h>  // need for memset
#include<stdio.h>   
#include<unistd.h>  // read/write
#include<stdlib.h>

void main () {


    struct sockaddr_in servAddr;
    int listenfd;

    /* prepare address info */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(MY_PORT_NUMBER);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);


    /* getting a text host name */



    /* create socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    /* bind address info to the socket */
    if ( bind( listenfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        perror("Bind error\n");
        exit(1);
    }
    
    /* listen for connections */
    listen(listenfd, 1);


    /* prepare for accepting an incoming connection */
    int connectfd;  // connection/communication FD 
    int length = sizeof(struct sockaddr_in);    
    struct sockaddr_in clientAddr;   // stores address info of the client */

    /* wait until a connection is establishd */
    connectfd = accept(listenfd, (struct sockaddr *) &clientAddr, &length);

    printf("Connection established!\n");

}

