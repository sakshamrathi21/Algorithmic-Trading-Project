//---------------------------------------
// DO NOT MODIFY THIS FILE
//---------------------------------------

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <unistd.h>



class Receiver
{
    const int PORT = 8826;
    const static int BUFFER_SIZE = 1024;

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddrLength;
    char buffer[BUFFER_SIZE];
    int bytesRead;

    public:
    Receiver()
    {
        // Create a Socket
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            std::cerr << "Error creating socket." << std::endl;
        }

        // Bind the socket to an address and port
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(PORT);


        if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
            std::cerr << "Error binding socket." << std::endl;
            close(serverSocket);
        }

        // Listen for incoming connections
        if (listen(serverSocket, 1) == -1) {
            std::cerr << "Error listening." << std::endl;
            close(serverSocket);
        }

        std::cout << "Receiver is listening on port " << PORT << std::endl;

        clientAddrLength = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddrLength);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection." << std::endl;
            close(serverSocket);
        }
    }


    std::string readIML()
    {
        // Receive data from the sender
        usleep(1000);
        bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead == -1) {
            std::cerr << "Error receiving data from sender." << std::endl;
            close(clientSocket);
            close(serverSocket);
        }

        buffer[bytesRead] = '\0'; // Null-terminate the received data

        std::string message = buffer;
        return message;
    }


    void terminate()
    {
        close(clientSocket);
        close(serverSocket);
    }
};
