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
#include<sys/wait.h>
#include<time.h>

void main () {


    struct sockaddr_in servAddr;
    int listenfd;

    /* prepare address info */
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(MY_PORT_NUMBER);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);


    /* getting a text host name */
    char hostname[1025];
    hostname[1024] = '\0';

    gethostname(hostname, 1024);            // this returns the ux-2
    //printf("hostname is %s\n", hostname);
    struct hostent* h;
    h = gethostbyname(hostname);            // this gets us the entire URL..and probably more
    //printf("h_name: %s\n", h->h_name);


    
    /* prepare for accepting an incoming connection */
    int connectfd;  // connection/communication FD 
    int length = sizeof(struct sockaddr_in);    
    struct sockaddr_in clientAddr;   // stores address info of the client */

    /* create socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    /* bind address info to the socket */
    if ( bind( listenfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        perror("Bind error\n");
        exit(1);
    }
    
    /* listen for connections */
    listen(listenfd, 3);

    time_t current_time;
    char *time_string;

    while (1) {

        /* wait until a connection is establishd */
        connectfd = accept(listenfd, (struct sockaddr *) &clientAddr, &length);
        if (fork()) {
            close(connectfd); 
            // quick zombie cleanup 
            for (int i = 0; i < 3; i++) {
                waitpid(-1, NULL, WNOHANG);
            }
        }
        else {

            /* establish client's hostname */
            char *name;
            name = inet_ntoa(clientAddr.sin_addr); 
            printf("established connection with: %s\n", name);

            /* determine message to send */
            time(&current_time);  
            time_string = ctime(&current_time);   
            //printf("current time is %s\n", time_string);

            //char msg[30] = "writing to client";
            write(connectfd, time_string, strlen(time_string));
            close(connectfd);
            exit(1);
        }
    }

    /* attempting to identify the client */
    /*
    socklen_t addrlen;
    char hbuf[1025]; char sbuf[30];

    if (getnameinfo((struct sockaddr *) &clientAddr, addrlen, hbuf, sizeof(hbuf),sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
        printf("name info problem\n");
    }
    else {
        printf("presumably got the info\n");
        printf("%s\n%s\n", hbuf, sbuf);
    }
    */

    close(listenfd);
   // inet_ntoa
    char *name; 
    name = inet_ntoa(clientAddr.sin_addr);
    printf("established connection with: %s?\n", name);
}

