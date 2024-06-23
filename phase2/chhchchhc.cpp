#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

int main() {
    std::string filename = "your_file.txt";

    while (true) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Failed to open the file!" << std::endl;
            return 1;
        }

        // Get the initial size of the file
        file.seekg(0, std::ios::end);
        std::streampos lastPosition = file.tellg();

        // Check if the file size has increased
        file.seekg(0, std::ios::end);
        std::streampos newPosition = file.tellg();

        if (newPosition > lastPosition) {
            // File has grown, read the new content
            file.clear(); // Clear any flags
            file.seekg(lastPosition);

            std::string line;
            while (std::getline(file, line)) {
                std::cout << line << std::endl;
            }

            lastPosition = file.tellg(); // Update last position to the end of the file
        }

        file.close(); // Close the file before sleeping
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}