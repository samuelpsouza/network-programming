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

	bzero(&simple_server, sizeof(simple_server));
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

	return EXIT_SUCCESS;
}
