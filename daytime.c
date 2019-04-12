/*
 *  Ryan Paulos
 *  CS 360, assignment 9 client
 */

#define MY_PORT_NUMBER 49999
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




int main (int argc, char* argv[]) {


    /*
    struct sockaddr_in servAddr;
    struct hostent* hostEntry;
    struct in_addr **pptr;

    memset( &servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(MY_PORT_NUMBER);

    char hostBuffer[256];
    int host_name = gethostname(hostBuffer, sizeof(hostBuffer));
    printf("host name is %s\n", hostBuffer);

    hostEntry = gethostbyname(hostBuffer);

    if (hostEntry == NULL) {
        herror("encountered error getting hostname\n");
        exit(1);
    }
    

    pptr = (struct in_addr **) hostEntry->h_addr_list;
    memcpy(&servAddr.sin_addr, *pptr, sizeof(struct in_addr));
    */

    /* create connection socket */
    int socketfd;
    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    /* prepare address info of the server */
    struct sockaddr_in servAddr;
    struct hostent* hostEntry;
    struct in_addr **pptr;
    //printf("here 1\n");
    memset( &servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(MY_PORT_NUMBER);
    //printf("here 1.5\n");

   
    /* first attempt to get server address using hostname */  
    if ( (hostEntry = gethostbyname(argv[1])) == NULL) {
        herror("invalid host name..attempting to use arg as IP address\n");
        /* now see if the arg is in dot notation */
        if (inet_pton(AF_INET, argv[1], &servAddr.sin_addr)<=0) {
            printf("both hostname and IP address failed\n");
            memset( &servAddr, 0, sizeof(servAddr));
            close(socketfd);
            exit(1);
        }
    }
    else {
        printf("hostname was fine\n");
        /* hostname sucess..copy data to appropriate structure */
        pptr = (struct in_addr **) hostEntry->h_addr_list;
        memcpy(&servAddr.sin_addr, *pptr, sizeof(struct in_addr));
    }
    

    //printf("here 2\n");
    // commenting this out. Testing gethostbyname rather than giving the ip address
    if (connect(socketfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        printf("Error: connection failed\n");
    }
    else {
        printf("connection established\n");
    }
    //printf("hello!!!!!!\n");
    char response[300] = {'\0'};
    read(socketfd, response, 299);
    printf("%s\n", response);
    close(socketfd);
}
