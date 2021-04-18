/*
 ============================================================================
 Name        : tcp-socket-server.c
 Author      : Samuel
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World Socket in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

static const int QUEUE_SIZE = 5;
const char SERVER_RESPONSE[] = "Hello World";

int main(int argc, char **argv) {
	if (2 != argc) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int simple_port = atoi(argv[1]);
	struct sockaddr_in simple_server;

	int simple_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (simple_socket == -1) {
		fprintf(stderr, "Could not create a socket!\n");
		exit(EXIT_FAILURE);
	} else {
		fprintf(stdout, "Socket created.\n");
	}

	memset(&simple_server, 0, sizeof(simple_server));
	simple_server.sin_family = AF_INET;
	simple_server.sin_addr.s_addr = htonl(INADDR_ANY);
	simple_server.sin_port = htons(simple_port);

	int bind_return_status = bind(simple_socket, (struct sockaddr*) &simple_server, sizeof(simple_server));
	if (bind_return_status == 0) {
		fprintf(stdout, "Bind completed.\n");
	} else {
		fprintf(stderr, "Could not bind to address.\n");
		close(simple_socket);
		exit(EXIT_FAILURE);
	}

	int listen_return_status = listen(simple_socket, QUEUE_SIZE);
	if(listen_return_status == 0){
		fprintf(stdout, "Server started to listen on port %s.\n", argv[1]);
	}else {
		fprintf(stderr, "Cannot listen on socket!\n");
		close(simple_socket);
		exit(EXIT_FAILURE);
	}

	while(1){
		struct sockaddr_in client_name = {0};
		int simple_client = 0;
		int client_name_len = sizeof(client_name);

		int simple_child_socket = accept(simple_socket, (struct sockaddr *) &client_name, &client_name_len);
		if(simple_client == -1){
			fprintf(stderr, "Cannot accept client connection.\n");
			close(simple_client);
			exit(EXIT_FAILURE);
		}

		write(simple_child_socket, SERVER_RESPONSE, strlen(SERVER_RESPONSE));
	}

	return EXIT_SUCCESS;
}
