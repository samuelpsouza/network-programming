#!/usr/bin/env python3
 
import argparse
import json
import base64
from os import path
import sys
import socket, ssl
from socket import socket as Socket
 
DEFAUTL_ENCODE = "utf-8"

def main():
 
    parser = argparse.ArgumentParser()
    parser.add_argument('my_address', type=str)
    parser.add_argument('mail_server', type=str)
    parser.add_argument('their_address', type=str)
    parser.add_argument('message', type=str)
    args = parser.parse_args()
 
 
    send_mail(args.my_address, args.mail_server, args.their_address, args.message)
     
 
    # success
    return 0

def print_reply_message(head, msg):
    print("{} {}".format(head, msg.decode("utf-8")))

    return 
 
def send_mail(my_address, mail_server, their_address, message):
 
    # Fill in the code to talk to the mail server here. Read the file
    # read-first.md to find out how to set up a mail server for testing. A log
    # of a telnet session with your mail server may be (very) useful here.
    
    server_addr = (mail_server, 465)
    user = None
    pwd = None

    filepath = path.abspath(path.join(path.dirname(__file__), 'credentials.json'))

    with open(filepath) as credentials:
        data = json.load(credentials)
        user = data['username']
        pwd = data['password']

    with ssl.wrap_socket(Socket(socket.AF_INET, socket.SOCK_STREAM), ssl_version=ssl.PROTOCOL_SSLv23) as client_socket:
        client_socket.connect(server_addr)
        res = client_socket.recv(1024)
        print_reply_message("Response for connection request:", res)

        client_socket.send("EHLO there\n".encode(DEFAUTL_ENCODE))
        res = client_socket.recv(1024)
        print_reply_message("Response for EHLO request:", res)

        client_socket.send("AUTH LOGIN\n".encode(DEFAUTL_ENCODE))
        res = client_socket.recv(1024)
        print_reply_message("Response for AUTH LOGIN request:", res)

        client_socket.send(base64.b64encode(user.encode(DEFAUTL_ENCODE)))
        client_socket.send("\r\n".encode(DEFAUTL_ENCODE))
        res = client_socket.recv(2048)
        print_reply_message("Response for user sent:", res)

        client_socket.send(base64.b64encode(pwd.encode(DEFAUTL_ENCODE)))
        client_socket.send("\r\n".encode(DEFAUTL_ENCODE))
        res = client_socket.recv(2048)
        print_reply_message("Response for pwd sent:", res)

        client_socket.send("MAIL FROM:<{}>\n".format(my_address).encode(DEFAUTL_ENCODE))
        res = client_socket.recv(1024)
        print_reply_message("Response for MAIL FROM request:", res)

        for addr in their_address:
            client_socket.send("RCPT TO:<{}>\n".format(addr).encode(DEFAUTL_ENCODE))
            res = client_socket.recv(1024)
                
        client_socket.send("DATA\n".encode(DEFAUTL_ENCODE))
        res = client_socket.recv(1024)
        print_reply_message("Response for DATA request:", res)

        client_socket.send("{}\n".format(message).encode(DEFAUTL_ENCODE))
        res = client_socket.recv(1024)
        print_reply_message("Response for sending message:", res)

        client_socket.send("QUIT\n".encode(DEFAUTL_ENCODE))
        res = client_socket.recv(1024)
        print_reply_message("Response for QUIT request:", res)
        
        client_socket.close()
    return
     
 
 
if __name__ == "__main__":
    sys.exit(main())