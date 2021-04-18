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
	int simple_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(simple_socket == -1){
		fprintf(stderr, "Could not create a socket!\n");
		exit(EXIT_FAILURE);
	} else {
		fprintf(stdout, "Socket created.\n");
	}

	return EXIT_SUCCESS;
}
