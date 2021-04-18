/*
 ============================================================================
 Name        : udp-socket-client.c
 Author      : Samuel
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#define MAXBUF 1024

int main(int argc, char **argv) {
	if(3 != argc){
		fprintf(stderr, "Usage: %s <server> <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int simple_port = atoi(argv[2]);
	struct sockaddr_in udp_server;
	struct sockaddr_in udp_client;

	int udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udp_socket == -1) {
		fprintf(stderr, "Could not create a socket!\n");
		exit(EXIT_FAILURE);
	} else {
		fprintf(stdout, "Socket created.\n");
	}

	udp_client.sin_family = AF_INET;
	udp_client.sin_addr.s_addr = INADDR_ANY;
	udp_client.sin_port = 0;

	int return_status = bind(udp_socket, (struct sockaddr *) &udp_client, sizeof(udp_client));
	if (return_status == 0) {
		fprintf(stdout, "Bind completed.\n");
	} else {
		fprintf(stderr, "Could not bind to address.\n");
		close(udp_socket);
		exit(EXIT_FAILURE);
	}

	char buf[MAXBUF];
	strcpy(buf, "Hello World");

	udp_server.sin_family = AF_INET;
	udp_server.sin_addr.s_addr = inet_addr(argv[1]);
	udp_server.sin_port = htons(simple_port);

	int read_return_status = sendto(udp_socket, buf, MAXBUF, 0, (struct sockaddr *) &udp_server, sizeof(udp_server));
	if(read_return_status == -1){
		fprintf(stderr, "Could not send message!\n");
	}else {
		fprintf(stdout, "Message sent.\n");
		int addr_len = sizeof(udp_server);
		return_status = recvfrom(udp_socket, buf, MAXBUF, 0, (struct sockaddr *) &udp_server, &addr_len);

		if (return_status == -1) {
			fprintf(stderr, "Did not receive confirmation.\n");
		}
		else {
			buf[return_status] = 0;
			fprintf(stdout, "Received: %s\n", buf);
		}
	}

	close(udp_socket);

	return EXIT_SUCCESS;
}
