// Listening to a given port no 8888 and printing the incoming messages
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <string>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>
#define ll int64_t

using namespace std;
//////////////////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%???????????????????????????????////////////////
struct trade_line {
    ll port;
    ll market_index;
    string brokerName;
    string companyName;
    ll quantity;
    string action;
    int package_price;
    bool participating = true;
    int start_time;
    int expiration_period;
};

int total_profit = 0 ;

void init_string(string &s){
    s = "";
}

void init_trade_line(trade_line &line){
    line.action = "";
    line.market_index = -1;
    line.package_price = 0;
}

void init_push_parameters(string &a, string &b, string &c, string &d){
    a = "";
    b = "";
    c = "";
    d = "";
}

void initialize_bool(bool &r1, bool &r2, bool &r3, bool &r4, bool &r5){
    r1 = false; r2 = false; r3 = false; r4 = false; r5 = false;
}

vector <string> all_broker_names;
vector <ll> port_numbers;
vector <trade_line> all_trades;

string brokerName = "22B1003_22B0933";

void trade_possible (int i , int j ) {
    if (all_trades[i].expiration_period < 0) {
        all_trades[i].expiration_period = -1;
    }
    if (all_trades[j].expiration_period < 0) {
        all_trades[j].expiration_period = -1;
    }
    if (!all_trades[i].participating || !all_trades[j].participating) return;
    if (all_trades[i].companyName != all_trades[j].companyName) return;
    if (all_trades[i].action == all_trades[j].action) return;
    if (all_trades[i].market_index == all_trades[j].market_index) {
        if (all_trades[i].action == "b") {
        // if (all_trades[j].action == "b") return;
            if (all_trades[j].package_price < all_trades[i].package_price) {
                if (all_trades[i].quantity > all_trades[j].quantity) {
                    all_trades[i].quantity -= all_trades[j].quantity;
                    all_trades[j].participating = false;
                }
                else if (all_trades[j].quantity > all_trades[i].quantity) {
                    all_trades[j].quantity -= all_trades[i].quantity;
                    all_trades[i].participating = false;
                }
                else {
                    all_trades[i].participating = false;
                    all_trades[j].participating = false;
                    all_trades[i].quantity = 0;
                    all_trades[j].quantity = 0;
                }
            }
        }
        else if (all_trades[i].action == "s") {
            if (all_trades[i].package_price < all_trades[j].package_price) {
                if (all_trades[i].quantity > all_trades[j].quantity) {
                    all_trades[i].quantity -= all_trades[j].quantity;
                    all_trades[j].participating = false;
                }
                else if (all_trades[j].quantity > all_trades[i].quantity) {
                    all_trades[j].quantity -= all_trades[i].quantity;
                    all_trades[i].participating = false;
                }
                else {
                    all_trades[i].participating = false;
                    all_trades[j].participating = false;
                    all_trades[i].quantity = 0;
                    all_trades[j].quantity = 0;
                }
            }
        }
        return;
    }
    
    
    if (all_trades[i].action == "b") {
        // if (all_trades[j].action == "b") return;
        if (all_trades[j].package_price < all_trades[i].package_price) {
            // cout<<i<<" i sell ; j buy "<<j <<endl;
            std::string filenamei = "output" + std::to_string(all_trades[i].market_index) + ".txt";
            std::ofstream outFilei;
            outFilei.open(filenamei, std::ios::out | std::ios::app);
            std::string filenamej = "output" + std::to_string(all_trades[j].market_index) + ".txt";
            std::ofstream outFilej;
            outFilej.open(filenamej, std::ios::out | std::ios::app);
            int starting_time = max(all_trades[i].start_time, all_trades[j].start_time);
            // if (starting_time == 7) {
            //     cout<<"CHECK 7 "<<all_trades[i].expiration_period<<all_trades[j].expiration_period;
            // }
            // all_trades[i].part
            if (all_trades[i].quantity > all_trades[j].quantity) {
                total_profit += all_trades[j].quantity*(all_trades[i].package_price - all_trades[j].package_price);
                all_trades[i].quantity -= all_trades[j].quantity;
                all_trades[j].participating = false;
                // string stringi = starting_time + " " << brokerName << " SELL " << all_trades[i].companyName << " $" << all_trades[i].package_price << " #" << all_trades[j].quantity << " " << all_trades[i].expiration_period << endl;
                
                outFilei<< starting_time << " SELL " << all_trades[i].companyName << " $" << all_trades[i].package_price << " #" << all_trades[j].quantity<<endl;
                outFilej<< starting_time<< " BUY " << all_trades[i].companyName << " $" << all_trades[j].package_price << " #" << all_trades[j].quantity<<endl;
            }
            else if (all_trades[j].quantity > all_trades[i].quantity) {
                total_profit += all_trades[i].quantity*(all_trades[i].package_price - all_trades[j].package_price);
                all_trades[j].quantity -= all_trades[i].quantity;
                all_trades[i].participating = false;
                outFilei<< starting_time<< " SELL " << all_trades[i].companyName << " $" << all_trades[i].package_price << " #" << all_trades[i].quantity<<endl;
                outFilej<< starting_time<< " BUY " << all_trades[i].companyName << " $" << all_trades[j].package_price << " #" << all_trades[i].quantity<<endl;
            }
            else {
                total_profit += all_trades[j].quantity*(all_trades[i].package_price - all_trades[j].package_price);
                all_trades[i].participating = false;
                all_trades[j].participating = false;
                all_trades[i].quantity = 0;
                all_trades[j].quantity = 0;
                outFilei<< starting_time<< " SELL " << all_trades[i].companyName << " $" << all_trades[i].package_price << " #" << all_trades[i].quantity<<endl;
                outFilej<< starting_time<< " BUY " << all_trades[i].companyName << " $" << all_trades[j].package_price << " #" << all_trades[i].quantity<<endl;
            }
        }
        else {
            return;
        }
    }
    else if (all_trades[i].action == "s") {
        if (all_trades[i].package_price < all_trades[j].package_price) {
            std::string filenamei = "output" + std::to_string(all_trades[i].market_index) + ".txt";
            std::ofstream outFilei;
            outFilei.open(filenamei, std::ios::out | std::ios::app);
            std::string filenamej = "output" + std::to_string(all_trades[j].market_index) + ".txt";
            std::ofstream outFilej;
            outFilej.open(filenamej, std::ios::out | std::ios::app);
            int starting_time = max(all_trades[i].start_time, all_trades[j].start_time);
            // if (starting_time == 7) {
            //     cout<<"CHECK 7 "<<all_trades[i].expiration_period<<all_trades[j].expiration_period;
            // }
            // all_trades[i].part
            if (all_trades[i].quantity > all_trades[j].quantity) {
                total_profit += all_trades[j].quantity*(all_trades[j].package_price - all_trades[i].package_price);
                all_trades[i].quantity -= all_trades[j].quantity;
                all_trades[j].participating = false;
             
                
                outFilei<< starting_time<< " BUY " << all_trades[i].companyName << " $" << all_trades[i].package_price << " #" << all_trades[j].quantity<<endl;
                outFilej<< starting_time<< " SELL " << all_trades[i].companyName << " $" << all_trades[j].package_price << " #" << all_trades[j].quantity<<endl;
            }
            else if (all_trades[j].quantity > all_trades[i].quantity) {
                total_profit += all_trades[i].quantity*(all_trades[j].package_price - all_trades[i].package_price);
                all_trades[j].quantity -= all_trades[i].quantity;
                all_trades[i].participating = false;
                outFilei<< starting_time<< " BUY " << all_trades[i].companyName << " $" << all_trades[i].package_price << " #" << all_trades[i].quantity<<endl;
                outFilej<< starting_time<< " SELL " << all_trades[i].companyName << " $" << all_trades[j].package_price << " #" << all_trades[i].quantity<<endl;
            }
            else {
                total_profit += all_trades[j].quantity*(all_trades[j].package_price - all_trades[i].package_price);
                all_trades[i].participating = false;
                all_trades[j].participating = false;
                all_trades[i].quantity = 0;
                all_trades[j].quantity = 0;
                outFilei<< starting_time<< " BUY " << all_trades[i].companyName << " $" << all_trades[i].package_price << " #" << all_trades[i].quantity<<endl;
                outFilej<< starting_time<< " SELL " << all_trades[i].companyName << " $" << all_trades[j].package_price << " #" << all_trades[i].quantity<<endl;
            }
        }
        else {
            return;
        }
    }
}

////////////////////////////////////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%???????????????????//////////////

const int BUFFER_SIZE = 1024;
int NUM_THREADS = 2;

// Structure to store client socket and its details
struct ClientInfo {
    int socket;
    struct sockaddr_in address;
    ClientInfo(int socket, struct sockaddr_in& address) : socket(socket), address(address) {}
    ClientInfo() {};
};

// Function to handle a client connection
void *handleClient(void *arg) {
    // port_numbers.push_back(1);
    ClientInfo *clientInfo = static_cast<ClientInfo *>(arg);
    int prevSize = 0;
    char buffer[BUFFER_SIZE];

    std::cout << "Connected to client, IP: " << inet_ntoa(clientInfo->address.sin_addr) << ", Port: " << ntohs(clientInfo->address.sin_port) << std::endl;

    while (true) {
        // Receive data from the client
        ssize_t bytesRead = recv(clientInfo->socket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            // Error or connection closed
            if (bytesRead == 0) {
                std::cout << "Connection closed by client, IP: " << inet_ntoa(clientInfo->address.sin_addr) << ", Port: " << ntohs(clientInfo->address.sin_port) << std::endl;
            } else {
                perror("Recv error");
            }
            break; 
        } else {
            string message = "";
            int ij = 0;
            buffer[bytesRead] = '\0';
            while(buffer[ij] != '\0'){
                message += buffer[ij];
                ij += 1;
            }
            
            
            ////////////////////////////////////////Message_Decode//////////////////////////////////////
            bool start_time_in, broker_name_in, action_in, equation_in, line_done;
            initialize_bool(start_time_in, broker_name_in, action_in, equation_in, line_done);

            trade_line *push_line = new trade_line;
            init_trade_line(*push_line);

            ll message_len = message.size();
            
            string s1 = "";    

            ll i = 0;                        
            int num_spaces = 0;
            push_line->port = ntohs(clientInfo->address.sin_port);
            while(i < message_len) {
                if(start_time_in == false){
                    if(message[i] == 36){break;}
                    else if(message[i] < 48 || message[i] > 57){
                        i += 1;
                        continue; 
                    }
                    s1 += message[i];
                    if(message[i + 1] == 32){ 
                        start_time_in = true;
                        push_line -> start_time =stoi(s1);
                        s1 = "";
                        i += 1;
                    }
                    i += 1;
                }
                else if(broker_name_in == false){
                    s1 += message[i];
                    if(message[i + 1] == 32){ 
                        broker_name_in = true;

                        bool broker_inserted = false;
                        for(ll g = 0; g < all_broker_names.size(); g++){
                            if(s1 == all_broker_names[g]){
                                broker_inserted = true;
                                break;
                            }
                        }
                        if(broker_inserted == false) all_broker_names.push_back(s1);

                        push_line -> brokerName = s1;
                        s1 = "";
                        i += 1;
                    }
                    i += 1;
                }
                else if(action_in == false){
                    s1 += message[i];
                    if(message[i + 1] == 32){ 
                        action_in = true;
                        if(s1 == "BUY") push_line->action = "b";
                        else push_line->action = "s";
                        s1 = "";
                        i += 1;
                    }
                    i += 1;
                }
                else if(equation_in == false){
                    if(num_spaces % 2 == 0){
                        if(message[i] == 36){
                            equation_in = true;
                            num_spaces = 0;
                            continue;
                        }
                        s1 += message[i];
                        if(message[i + 1] == 32){ 
                            push_line->companyName = s1;
                            s1 = "";
                            i += 1;
                        }
                        i += 1;
                    }
                }
                else if(line_done == false){
                    if(message[i] == 36 || message[i] == 35) i += 1;
                    else if(num_spaces == 0){
                        s1 += message[i];
                        if(message[i+1] == 32){
                            push_line->package_price = stoi(s1);
                            s1 = "";
                            i += 1;
                            num_spaces += 1;
                        }
                        i += 1;
                    }
                    else if(num_spaces == 1){
                        s1 += message[i];
                        if(message[i+1] == 32){
                            push_line->quantity = stoi(s1);
                            s1 = "";
                            i += 1;
                            num_spaces += 1;
                        }
                        i += 1;
                    }
                    else if(num_spaces == 2){
                        s1 += message[i];
                        // will have to change wrt to \n or something else at end of each line
                        if(message[i+1] > 57 || message[i + 1] < 48){
                            push_line->expiration_period = stoi(s1);
                            all_trades.push_back(*push_line);
                            init_trade_line(*push_line);
                            initialize_bool(start_time_in, broker_name_in, action_in, equation_in, line_done);
                            s1 = "";
                            i += 1;
                            num_spaces = 0;
                        }
                        i += 1;
                    }
                }
            }    

                
            for(int j = prevSize; j < all_trades.size(); j++){
                for(int z = 0; z < port_numbers.size(); z++){
                    if(port_numbers[z] == (all_trades[j]).port){
                        (all_trades[j]).market_index = z + 1;
                        break;
                    }
                }
                if((all_trades[j]).market_index == -1){
                    (all_trades[j]).market_index = port_numbers.size() + 1;
                    port_numbers.push_back((all_trades[j]).port);
                }
            }

            for (int i = prevSize ; i < all_trades.size() ; i ++ ){
                for (int j = 0 ; j < i ; j ++ ) {
                    if (all_trades[j].participating == true && all_trades[j].expiration_period != -1 && all_trades[j].start_time + all_trades[j].expiration_period < all_trades[i].start_time) {
                        all_trades[j].participating = false;
                    }
                    trade_possible(i, j);
                }
            }
            prevSize = all_trades.size();

            ///////////////////////////////////////////////\/\/\/\/\/////////////////////////////////////////
            // Print the received message
            buffer[bytesRead] = '\0';
            std::cout << "Received message from client, IP: " << inet_ntoa(clientInfo->address.sin_addr) << ", Port: " << ntohs(clientInfo->address.sin_port) << ": " << buffer << std::endl;
        }
    }
    
    // Close the client socket
    close(clientInfo->socket);
    delete clientInfo;
    pthread_exit(NULL);
}

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create server socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);  // Port number
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind server socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {  // Maximum 5 pending connections
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    std::cout << "Trader is listening on port 8888..." << std::endl;

    std::vector<pthread_t> clientThreads;

    for(int i = 0; i < NUM_THREADS; i++) {
        // Accept incoming connections
        int clientSocket;
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen)) == -1) {
            perror("Accept error");
            continue;  // Continue listening for other connections
        }

        // Create a thread to handle this client
        ClientInfo *clientInfo = new ClientInfo(clientSocket, clientAddr);
        pthread_t clientThread;
        if (pthread_create(&clientThread, NULL, handleClient, clientInfo) != 0) {
            perror("Thread creation error");
            delete clientInfo;
            continue;  // Continue listening for other connections
        }

        // Store the thread ID for later joining
        clientThreads.push_back(clientThread);
    }

    // Join all client threads (clean up)
    for (auto &thread : clientThreads) {
        pthread_join(thread, NULL);
    }

    // Close the server socket (never reached in this example)
    close(serverSocket);
    std::cout<<total_profit<<endl;
    return 0;
}