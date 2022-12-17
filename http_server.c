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

char* copy(char* dest, char* src)
{
    int pointer = 0;
    while( src[pointer] != '\0')
    {
        dest[pointer]=src[pointer];
        pointer++;
    }
    dest[++pointer]='\0';
    return dest;
}
char* copy2(char* dest, char* src, int count)
{
    int src_pointer = 0;
    while(count >= 0)
    {
        dest[src_pointer]=src[src_pointer];
        src_pointer++;
        count--;
    }
    dest[++src_pointer]='\0';
    return dest;
}

// from client:
// GET /page1 HTTP/1.1 (1)
// Host: 127.0.0.1:8080 (2)
// User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:106.0) Gecko/20100101 Firefox/106.0 (3)
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8 (4)
// Accept-Language: en-US,en;q=0.5 (5)
// Accept-Encoding: gzip, deflate, br (6)
// Connection: keep-alive (7)
struct http_request {
    char* http_method;
    char* endpoint;
    char* host;
    char* user_agent;
    char* accept;
    char* accept_language;
    char* accept_encoding;
    char* connection;
};

// How about we assume that the order/format of http
// headers in the request is always same and fail with
// exceptions with tests!
// TODO: find out if the order is different from other clients.
struct http_request parse_client_message(char *client_message)
{
    struct http_request hpr;
    printf("inside parse client message method\n");
    int client_message_pointer = 0;
    int number_of_lines = 0;
    char current_character;
    char* word = (char *) malloc(100*sizeof(char));
    int word_pointer = 0;
    int word_count = 0;
    while (client_message[client_message_pointer] != '\0')
    {
        current_character = client_message[client_message_pointer++];

        if (current_character == 32) // space
        {
            word_count++;
            if (number_of_lines + 1  == 1)
            {
                if (word_count == 1)
                {
                    // GET
                    word[word_pointer]='\0';
                    word_pointer=0;
                    hpr.http_method = (char*) malloc(9*sizeof(char));
                    copy(hpr.http_method, word);
                }
                else if (word_count == 2)
                {
                    // /endpoint
                    word[word_pointer]='\0';
                    word_pointer=0;
                    hpr.endpoint = (char*) malloc(15*sizeof(char));
                    copy(hpr.endpoint, word);
                }
                else
                {
                    continue;
                }
            }
            else
            {
                if (word_count == 1)
                {
                    word_pointer=0;
                    word_count++;
                }
                else
                {
                    continue;
                }
            }
        }
        else if (current_character == 13) // carriage return
        {
            number_of_lines++;
            word[word_pointer] = '\0';
            if (number_of_lines > 1)
            {
                if (number_of_lines == 2)
                {
                    // host
                    hpr.host = (char*) malloc(30*sizeof(char));
                    copy(hpr.host, word);
                    word_pointer=0;
                    word_count=0;
                }
                else if (number_of_lines == 3)
                {
                    // user-agent
                    hpr.user_agent = (char*) malloc(85*sizeof(char));
                    copy(hpr.user_agent, word);
                    word_pointer=0;
                    word_count=0;
                }
                else if (number_of_lines == 4)
                {
                    // accept
                    hpr.accept = (char*) malloc(85*sizeof(char));
                    copy(hpr.accept, word);
                    word_pointer=0;
                    word_count=0;
                }
                else if (number_of_lines == 5)
                {
                    // accept-language
                    hpr.accept_language = (char*) malloc(30*sizeof(char));
                    copy(hpr.accept_language, word);
                    word_pointer=0;
                    word_count=0;
                }
                else if (number_of_lines == 6)
                {
                    // accept-encoding
                    hpr.accept_encoding = (char*) malloc(30*sizeof(char));
                    copy(hpr.accept_encoding, word);
                    word_pointer=0;
                    word_count=0;
                }
                else if (number_of_lines == 7)
                {
                    // connection
                    hpr.connection = (char*) malloc(15*sizeof(char));
                    copy(hpr.connection, word);
                    word_pointer=0;
                    word_count=0;
                }
                else
                {
                    // just keep resetting word and line numbers
                    word_count=0;
                    word_pointer=0;
                }

            }
            else
            {
                word_count=0;
                word_pointer=0;
            }

        }
        else if (current_character == 10) // line feed or new line
        {
            continue;
        }
        else // normal characters
        {
            word[word_pointer++] = current_character;
        }
    }

    printf("number of lines %d\n", number_of_lines);
    printf("number of words %d\n", word_count);
    free(word);
    printf("freeing word\n");
    printf("outside parse client message\n");
    return hpr;
}

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
        printf("from client:\n%s\n", client_message);
        struct http_request hp = parse_client_message(client_message);

        printf("%s\n %s\n %s\n %s\n %s\n %s\n %s\n %s\n", hp.host, hp.http_method, hp.endpoint, hp.connection, hp.user_agent, hp.accept, hp.accept_language, hp.accept_encoding);
        FILE *fp;

        int blob_factor = 1;
        //FILE *fp = fopen("hello.html", "r");
        if(strcmp(hp.endpoint, "/page1") == 0 || strcmp(hp.endpoint, "/page1?") == 0)
        {
            fp = fopen("page1.html", "r");
        }
        else if(strcmp(hp.endpoint, "/page2") == 0 || strcmp(hp.endpoint, "/page2?") == 0)
        {
            fp = fopen("page2.html", "r");
        }
        else
        {
            fp = fopen("login_page.html", "r");
        }

        char *buffer, *login_response;
        buffer = malloc(blob_factor*BLOB*sizeof(char));
        struct file_contents login_file_contents = get_from(fp, buffer, 1, BLOB);
        
        login_response = malloc(blob_factor*BLOB*sizeof(char));
        printf("initialized login_page with %d units.\n", BLOB*blob_factor);


        //char http_header[] = "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 98\nContent-Type: text/html\n\n\n";
        // http headers template
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
        http_headers[9] = convert_to_string(login_file_contents.size); // malloc inside!
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
            if (i%2 == 0)
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

        // append html file response to the http response template
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
        free(http_headers[9]);
        printf("free http_headers[9]");
        printf("freeing connection\n");
        free(hp.connection);
        printf("freeing accept encoding\n");
        free(hp.accept_encoding);
        printf("freeing accept language\n");
        free(hp.accept_language);
        printf("freeing accept\n");
        free(hp.accept);
        printf("freeing user agent\n");
        free(hp.user_agent);
        printf("freeing endpoint\n");
        free(hp.endpoint);
        printf("freeing http method\n");
        free(hp.http_method);
        printf("freeing host\n");
        free(hp.host);

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


