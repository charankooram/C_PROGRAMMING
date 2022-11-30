#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 800
#define PORT 8080
#define SA struct sockaddr

void transactions(int connfd)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;)
    {
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        // print buffer which contains client message
        printf("from client: %s", buff);
        //bzero(buff, MAX);
        //n = 0;
        // copy server message in buffer
        //while ((buff[n++] = getchar()) != '\n')
        //    ;
        char response[] = "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 48\nContent-Type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>";
        // and send that buffer to client
        printf("%d\n", sizeof(response));
        printf("sending reponse as %s", response);
        write(connfd, response, sizeof(response));
        printf("sent response.. listening again!");

        // if msg contains exit then server exits
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("server exit\n");
            break;
        }

    }
}

// driver function
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    printf("hello world!\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //printf("%s", sockfd);

    // socket create and verification
    if (sockfd == -1)
    {
        printf("socket  creation failed\n");
        exit(0);
    }
    else
        printf("socket successfully created\n");
    bzero(&servaddr, sizeof(servaddr));
    printf("servaddr %s\n", servaddr);
    
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // printf("%s, %s, %s", servaddr.sin_family, servaddr.sin_addr.s_addr, servaddr.sin_port);
   
    // Bind newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0 )
    {
        printf("socket bind failed\n");
        exit(0);
    }
    else
        printf("socket successfully bound\n");

    // Now server is ready to listen and verify
    if ( (listen(sockfd, 5)) != 0) 
    {
        printf("listen failed\n");
        exit(0);
    }
    else
        printf("server listening\n");
    len = sizeof(cli);

    // accept the data packet from client and verify
    connfd = accept(sockfd, (SA*)&cli, &len);
    if ( connfd < 0 )
    {
        printf("server accept failed\n");
        exit(0);
    }
    else
        printf("server accept the client\n");
    
    // function for chatting between client and server
    transactions(connfd);

    // after chatting close the socket
    close(sockfd);

}


