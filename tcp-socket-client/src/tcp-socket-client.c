/*
 ============================================================================
 Name        : tcp-socket-client.c
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


int main(int argc, char **argv) {
	if(3 != argc){
		fprintf(stderr, "Usage: %s <server> <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int simple_port = atoi(argv[2]);
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
	inet_addr(argv[1], &simple_server.sin_addr.s_addr);
	simple_server.sin_port = htons(simple_port);

	int connect_return_status = connect(simple_socket, (struct sockaddr *) &simple_server, sizeof(simple_server));
	if (connect_return_status == 0) {
		fprintf(stdout, "Connected successfully.\n");
	} else {
		fprintf(stderr, "Could not connect to address.\n");
		close(simple_socket);
		exit(EXIT_FAILURE);
	}

	char buffer[256] = "";
	int read_return_status = read(simple_socket, buffer, sizeof(buffer));
	if(read_return_status > 0){
		printf("Response: %s", buffer);
	}else {
		fprintf(stderr, "Return Status = %d \n", read_return_status);
	}

	close(simple_socket);

	return EXIT_SUCCESS;
}
