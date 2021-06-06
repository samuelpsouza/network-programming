#!/usr/bin/env python3
 
import sys
import argparse
 
import timeit
import socket
from socket import socket as Socket
 
def main():
 
    # Command line arguments. Use a server_port > 1024 by default so that we can run
    # server without sudo.
    parser = argparse.ArgumentParser()
     
    parser.add_argument('--server-port', '-p', default=2081, type=int,
                        help='Server_Port to use')
     
    parser.add_argument('--run-server', '-s', action='store_true',
                        help='Run a ping server')
     
    parser.add_argument('server_address', default='localhost',
                        help='Server to ping, no effect if running as a server.')
     
    args = parser.parse_args()
 
 
    if args.run_server:
        return run_server(args.server_port)
    else:
        return run_client(args.server_address, args.server_port,)
 
 
 
 
def run_server(server_port):
    """Run the UDP pinger server
    """

    reply_message = "Pong"

    # Create the server socket (to handle UDP requests using ipv4), make sure
    # it is always closed by using with statement.
    with Socket(socket.AF_INET, socket.SOCK_DGRAM) as server_socket:
 
        # The socket stays connected even after this script ends. So in order
        # to allow the immediate reuse of the socket (so that we can kill and
        # re-run the server while debugging) we set the following option. This
        # is potentially dangerous in real code: in rare cases you may get junk
        # data arriving at the socket.
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
 
        # Set the server port
        server_socket.bind(('', server_port))
 
        # Start accepting ping requests
        print("Ping server ready on port", server_port)
        while True:
            # Receive message and send one back
            message, client_address = server_socket.recvfrom(1024)
            server_socket.sendto(reply_message.encode(), client_address)

            print("Message received from client {}: {}".format(client_address, message.decode("utf-8")))
 
    return 0

def print_rtt(index, start_time, end_time):
    print("RTT-{}: {}".format(index, end_time-start_time))

def run_client(server_address, server_port):
    """Ping a UDP pinger server running at the given address
    """

    ping_message = "Ping"

    # Fill in the client side code here.
    for i in range(0, 10):
        with Socket(socket.AF_INET, socket.SOCK_DGRAM) as client_socket:
            start_time = timeit.default_timer()

            client_socket.sendto(ping_message.encode(), (server_address, server_port))
            message, client_address = client_socket.recvfrom(1024)
            end_time = timeit.default_timer()

            print_rtt(i, start_time, end_time)
            print("Message received from server {}: {}".format(client_address, message.decode("utf-8")))
    return 0
 
if __name__ == "__main__":
    sys.exit(main())