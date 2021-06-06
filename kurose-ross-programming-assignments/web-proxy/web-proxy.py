#!/usr/bin/env python3
 
import argparse
import random
import sys
import socket
from socket import socket as Socket

CACHE_DIRECTORY = "/tmp/"

def get_host_port(resource):
    port = 80
    host = resource.split("://")[1].split("/")[0]
    return (host, port)

def main():
 
    # Command line arguments. Use port 8080 by default: widely used for proxys
    # and >1024 so we don't need sudo to run.
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', '-p', default=8080, type=int, help='Port to use')
    args = parser.parse_args()
 
 
    # Create the server socket (to handle tcp requests using ipv4), make sure
    # it is always closed by using with statement.
    with Socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
 
        # The socket stays connected even after this script ends. So in order
        # to allow the immediate reuse of the socket (so that we can kill and
        # re-run the server while debugging) we set the following option. This
        # is potentially dangerous in real code: in rare cases you may get junk
        # data arriving at the socket.
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
 
        server_socket.bind(('', args.port))
        server_socket.listen(1)
        # no multithreaded yet, would need to set up atomic updates to dict.
        # Might be automatic in python?
 
        # Create empty dict for cached pages
        cache_dict = {}
 
        print("Proxy server ready")
 
        while True:
            # Accept TCP connection from client
            with server_socket.accept()[0] as connection_socket:
                request = connection_socket.recv(1024)

                # Fill in the code to recive the request, check if the url is
                # in cache_dict and either serve the cached version or request
                # the page from the real server and cache it.
 
                # You may want to use code from the web server to extract
                # information from the request.
 
                # If you want to do more after that you could try to handle
                # updating cached pages, and then try to convert the server to
                # a multithreaded version.

                resource = request.decode('ascii').split(" ")[1]

                if cache_dict.get(resource) != None:
                    print("The resource is cached: {}".format(cache_dict.get(resource)))
                    file = open(cache_dict.get(resource), 'rb')

                    byte = file.read(1)
                    while byte:
                        connection_socket.send(byte)
                        byte = file.read(1)
                else:
                    print("The resource is not cached: {}".format(cache_dict.get(resource)))
                    filename = str(random.randint(0, sys.maxsize))
                    filename = CACHE_DIRECTORY + filename
                    cache_dict[resource] = filename
                    
                    host, port = get_host_port(resource)

                    with Socket(socket.AF_INET, socket.SOCK_STREAM) as external_connection:
                        external_connection.connect((host, port))
                        external_connection.sendall(request)

                        while(True):
                            data = external_connection.recv(1024)

                            if(len(data) > 0):
                                connection_socket.send(data)
                                file = open(filename, 'ab')
                                file.write(data)
                                file.close()
                            else:
                                break

    return 0
 
 
if __name__ == "__main__":
    sys.exit(main())