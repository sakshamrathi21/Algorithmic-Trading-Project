//---------------------------------------
// DO NOT MODIFY THIS FILE
//---------------------------------------

// Listening to a given port no 8888 and printing the incoming messages
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <atomic>
#include <fstream>
#include <string>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>

int NUM_THREADS = 2;
const int BUFFER_SIZE = 1024;
#define PORT 8888
// std::atomic<int> commonTimer = 0; // Shared timer
std::atomic<int> commonTimer(0); // For MAC/Unix

void* workerThread(void* arg) {
    int thread_id = *(int*)arg;
    // creating a socket and joining the server and sending the contents of the file
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
     
    }

    // Connect to the receiver
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to receiver." << std::endl;
        close(clientSocket);
   
    }

    std::cout << "Connected to the receiver." << std::endl;
    std::ifstream inputFile("markets/market" + std::to_string(thread_id) + ".txt"); 

    char buffer[BUFFER_SIZE];
    int bytesSent;
    std::string message;
    while(std::getline(inputFile, message)) {
        int currentTime;
        {
            currentTime = commonTimer.load();
        }
        message = std::to_string(currentTime) + " " + message;
        // Send the message to the receiver
        bytesSent = send(clientSocket, message.c_str(), message.length(), 0);
        if (bytesSent == -1) {
            std::cerr << "Error sending data to receiver." << std::endl;
            close(clientSocket);
        }    

        if (thread_id == 1)
        commonTimer.fetch_add(1);

        // std::cout << thread_id << " " << message << std::endl;

        usleep(500000);
    }

    message = "$";

    // Send the message to the receiver
    bytesSent = send(clientSocket, message.c_str(), message.length(), 0);
    if (bytesSent == -1) {
        std::cerr << "Error sending data to receiver." << std::endl;
        close(clientSocket);
    }

    // Close the client socket
    close(clientSocket);
    return NULL;
}



int main(int argc, char* argv[]) {
    
NUM_THREADS = std::stoi(argv[1]);
std::vector<pthread_t> clientThreads;
for (int i=0;i<NUM_THREADS;i++){
        int* arg = new int;
        *arg = i + 1;
        pthread_t clientThread;
        if (pthread_create(&clientThread, NULL, workerThread,(void*) arg) != 0) {
            perror("Thread creation error");
            continue;  // Continue listening for other connections
        }
    // Store the thread ID for later joining
        clientThreads.push_back(clientThread);
    }
    // Join all client threads (clean up)
    for (auto &thread : clientThreads) {
        pthread_join(thread, NULL);
    }
    return 0;
}
