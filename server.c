#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

/*
* Encapsulates the properties of the server.
*/
typedef struct server {
	// file descriptor of the socket in passive 
	// mode to wait for connections.
	int listen_fd;
} server_t;

/*
* Creates a socket for the server and makes it passive
* such that we can wait for connections on it later.
*/
int server_listen(server_t* server);
/*
* Accepts new connections and then prints `Hello World` to them
*/
int server_accept(server_t* server);

#define PORT 8123

/*
* Main server routine.
* - instantiate a new server structure that holds the properties
* of our server;
* - creates socket and makes it passive with `server listen`;
* - accepts new connections on the server socket.
*/
int main(){
	printf("started server program\n");
	int err = 0;
	server_t server = { 0 };
	printf("before server listen\n");
	err = server_listen(&server);
	//printf("after server listen");
	if(err) {
		printf("Failed to listen on address 0.0.0.0:%d\n", PORT);
		return err;
	}
	for(;;){
		//printf("before server accepting");
		err = server_accept(&server);
		//printf("after server accepting");
		if(err){
			printf("Failed accepting connection\n");
			return err;
		}
	}

	return 0;
}

int server_listen(server_t* server){
	int err = (server->listen_fd = socket(AF_INET, SOCK_STREAM,0));
	//pause();
	if (err == -1) {
		perror("socket");
		printf("Failed to create socket endpoint\n");
		return err;
	}
	//struct sockaddr_in server_addr = { 0 };
	//server_addr.sin_family = AF_INET;
	//server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//server_addr.sin_port = htons(PORT);

	//printf("%s\n", server_addr.sin_addr.s_addr);
	//printf("%s\n", server_addr.sin_port);

	//err = bind(server->listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	//if (err == -1) {
	//	perror("bind");
	//	printf("Failed to bind socket to address\n");
	//	return err;
	//}
	//printf(err);
}

int server_accept(server_t* server){
	int err = (server->listen_fd = socket(AF_INET, SOCK_STREAM,0));
	if (err == -1){
		perror("socket");
		printf("Failed to create socket endpoint\n");
		return err;
	}
}



