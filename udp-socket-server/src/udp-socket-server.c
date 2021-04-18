/*
 ============================================================================
 Name        : udp-socket-server.c
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
	if (2 > argc) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int port = atoi(argv[1]);
	struct sockaddr_in udp_server;
	struct sockaddr_in udp_client;

	int udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udp_socket == -1) {
		fprintf(stderr, "Could not create a socket!\n");
		exit(EXIT_FAILURE);
	} else {
		fprintf(stdout, "Socket created.\n");
	}

	udp_server.sin_family = AF_INET;
	udp_server.sin_addr.s_addr = htonl(INADDR_ANY);
	udp_server.sin_port = htons(port);

	int bind_return_status = bind(udp_socket, (struct sockaddr*) &udp_server, sizeof(udp_server));
	if (bind_return_status == 0) {
		fprintf(stdout, "Bind completed.\n");
	} else {
		fprintf(stderr, "Could not bind to address.\n");
		close(udp_socket);
		exit(udp_socket);
	}

	int addr_len = 0;
	char buf[MAXBUF];
	int return_status = 0;

	while(1){
		addr_len = sizeof(udp_client);
		return_status = recvfrom(udp_socket, buf, MAXBUF, 0, (struct sockaddr *) &udp_client, &addr_len);

		if(return_status == -1){
			fprintf(stderr, "Could not receive message.\n");
		}else {
			printf("Received: %s\n", buf);

			strcpy(buf, "OK");

			return_status = sendto(udp_socket, buf, MAXBUF, 0, (struct sockaddr *) &udp_client, sizeof(udp_client));
			if(return_status == -1){
				fprintf(stderr, "Could not send confirmation.\n");
			}else{
				fprintf(stdout, "Confirmation sent.\n");
			}
		}
	}

	close(udp_socket);
	return EXIT_SUCCESS;
}
