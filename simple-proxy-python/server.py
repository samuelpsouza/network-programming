from socket import *

serverPort=8080
serverSocket=socket(AF_INET,SOCK_STREAM)
serverSocket.bind(('',serverPort))
serverSocket.listen(1)
connectionSocket, addr = serverSocket.accept()

while 1:
    request = connectionSocket.recv(1024)
    
    port = 80
    dest = request.decode('ascii').split(" ")[1]
    host = dest.split("://")[1].split("/")[0]

    print("Proxing to {}".format(dest))

    with socket(AF_INET, SOCK_STREAM) as external_connection:
        external_connection.connect((host, port))
        external_connection.sendall(request)

        while(True):
            data = external_connection.recv(1024)

            if(len(data) > 0):
                connectionSocket.send(data)
            else:
                break

serverSocket.close()