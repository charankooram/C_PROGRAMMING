#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "DynamicMemoryAllocationTest.h"
#include "utility.h"

#define MAX 800
#define PORT 8080
#define SA struct sockaddr
#include "file_reader.h"
#define BLOB 500



void transactions(int connfd)
{
    char client_message[MAX];
    int n;
    // infinite loop for chat
    for (;;)
    {
        bzero(client_message, MAX);

        // read the message from client and copy it in buffer
        read(connfd, client_message, sizeof(client_message));
        // print buffer which contains client message
        printf("from client: %s\n", client_message);
        int blob_factor = 1;
        FILE *fp = fopen("hello.html", "r");

        char *buffer, *login_response;
        buffer = malloc(blob_factor*BLOB*sizeof(char));
        struct file_contents login_file_contents = get_from(fp, buffer, 1, BLOB);
        
        login_response = malloc(blob_factor*BLOB*sizeof(char));
        printf("initialized login_page with %d units.\n", BLOB*blob_factor);


        //char http_header[] = "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 98\nContent-Type: text/html\n\n\n";
        char* http_headers[] = {
                "HTTP/1.1",
                "200 OK",
                "Date:",
                "Mon, 27 Jul 2009 12:28:53 GMT",
                "Server:",
                "Apache/2.2.14 (Win32)",
                "Last-Modified:",
                "Wed, 22 Jul 2009 19:15:56 GMT",
                "Content-Length:",
                "98",
                "Content-Type:",
                "text/html"
        };
        // update content type with no. of chars from html file
        http_headers[9] = convert_to_string(login_file_contents.size);
        int http_header_size = 0;
        int http_header_offset = 0;

        // copy headers to login_response
        // something is wrong here !
        for(int i=0;i<12;i++)
        {
            int local_word_pointer=0;
            while(http_headers[i][local_word_pointer]!='\0')
            {
                login_response[http_header_offset++] = http_headers[i][local_word_pointer++];
            }
            if (i/10 ==0)
            {
                login_response[http_header_offset++] = ' ';
            }
            else
            {
                login_response[http_header_offset++] = '\n';
            }
        }

        // add two additional new line characters
        login_response[http_header_offset++] = '\n';
        login_response[http_header_offset++] = '\n';
        printf("copied http headers to login_response. size %d offset %d\n", http_header_size, http_header_offset);


        for (int i=0; i<login_file_contents.size; i++)
        {
            printf("%c", login_file_contents.buffer[i]);
            if (http_header_offset < blob_factor*BLOB)
            {
                login_response[http_header_offset++] = login_file_contents.buffer[i];
            }
            else
            {
                login_response = realloc(login_response, ++blob_factor*BLOB*sizeof(char));
                printf("resizing buffer to %d units.\n", blob_factor*BLOB);
                login_response[http_header_offset++] = login_file_contents.buffer[i];
            }
        }
        printf("\ncopied login file contents to login response %d\n", http_header_offset);

        // printing response to console for confirmation.
        int login_response_pointer = 0;
        while (login_response_pointer < http_header_offset)
        {
            printf("%c", login_response[login_response_pointer++]);
        }
        printf("\n");

        //char answer[] = "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 98\nContent-Type: text/html\n\n\n<html>\n<head>\n<title>C-Style Services</title>\n</head>\n<body>\n<p1>HELLO WORLD!</p1>\n</body>\n</html>";

        // and send that buffer to client
        int status = write(connfd,login_response, http_header_offset);
        printf("server write back status %d\n", status);

        free(login_file_contents.buffer); // what about buffer ? Should it be freed too ?
        login_file_contents.size = 0;
        printf("freeing login_file_contents\n");
        free(login_response);
        printf("freeing login_response\n");
        fclose(fp);
        printf("Closing hello.html page\n");

        printf("\n\nsent response.. listening again!\n\n");

        // if msg contains exit then server exits
        if (strncmp("exit", client_message, 4) == 0)
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
    if ((bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) != 0 )
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
    connfd = accept(sockfd, (struct sockaddr *) &cli, &len);
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


