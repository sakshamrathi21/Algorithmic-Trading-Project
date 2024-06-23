//---------------------------------------
// DO NOT MODIFY THIS FILE
//---------------------------------------

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>

const int PORT = 8826;
const int BUFFER_SIZE = 1024;

int main(int argc, char **argv) {
    std::string filepath = argv[1];

    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Connect to the receiver
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to receiver." << std::endl;
        close(clientSocket);
        return 1;
    }

    std::cout << "Connected to the receiver." << std::endl;

    std::ifstream inputFile(filepath); 

    char buffer[BUFFER_SIZE];
    int bytesSent;
    std::string message;

    while(std::getline(inputFile, message)) {
        // Send the message to the receiver
        bytesSent = send(clientSocket, message.c_str(), message.length(), 0);
        if (bytesSent == -1) {
            std::cerr << "Error sending data to receiver." << std::endl;
            close(clientSocket);
            return 1;
        }
        
    }

    message = "$";

    // Send the message to the receiver
    bytesSent = send(clientSocket, message.c_str(), message.length(), 0);
    if (bytesSent == -1) {
        std::cerr << "Error sending data to receiver." << std::endl;
        close(clientSocket);
        return 1;
    }

    // Close the client socket
    close(clientSocket);

    return 0;
}
