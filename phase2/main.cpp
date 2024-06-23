#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>
#include "market.h"


int NUM_THREADS = 50; // Number of worker threads
extern const int NUM_STEPS;  // Number of time steps

// std::atomic<int> commonTimer = 0; // Shared timer
std::atomic<int> commonTimer(0); // For MAC/UNIX
std::mutex printMutex; // Mutex for thread-exclusive printing

// Function to simulate work for each worker thread
extern void* workerThread(void* arg);
extern int reader(int time);
extern int trader(std::string* message);

void* userThread(void* arg)
{
    int thread_id = *(int*)arg;
    while(true)
    {
        int currentTime;
        {
            currentTime = commonTimer.load();
        }
        int end = reader(currentTime);
        if (end) break;
    }
    return NULL;
}

void* userTrader(void* arg)
{
    int thread_id = *(int*)arg;
    std::string message;
    int valid = trader(&message);
    int currentTime;
    {
        currentTime = commonTimer.load();
    }
    if (valid) std::cout << currentTime << " " << message << std::endl;
    return NULL;
}

int main(int argc, char** argv) {
    std::cout << "TL" << std::endl;
    std::vector<pthread_t> clientThreads;
    for (int i=0;i<NUM_THREADS;i++){
        int* arg = new int;
        *arg = (i+1);
        pthread_t clientThread;
        if (pthread_create(&clientThread, NULL, &workerThread,(void*) arg) != 0) {
            perror("Thread creation error");
            continue;  // Continue listening for other connections
        }
    // Store the thread ID for later joining
        clientThreads.push_back(clientThread);
    }

    pthread_t clientThread;
    if (pthread_create(&clientThread, NULL, &userThread,(void*) &NUM_THREADS) != 0) {
        perror("Thread creation error");
    }
    clientThreads.push_back(clientThread);

    // Join all worker threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(clientThreads[i], NULL);
    }

    std::cout << "!@" << std::endl; // Indicates the end of market input

    pthread_join(clientThreads[NUM_THREADS], NULL);

    std::cout << "@!" << std::endl; // Indicates the end of user input

    market marketMaker(argc, argv);
    std::cout << "Successfully Initiated!" << std::endl;
    marketMaker.start();

    return 0;
}