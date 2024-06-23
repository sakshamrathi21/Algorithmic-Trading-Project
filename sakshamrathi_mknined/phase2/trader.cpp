// This is your trader. Place your orders from here
#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <atomic>
#include <mutex>
#include<vector>
#define ll int64_t

using namespace std;
extern std::atomic<int> commonTimer;
extern std::mutex printMutex;
vector<string> all_lines;

/////////////////////////// ALL FUNCTIONS ///////////////////////////
vector <string> stock_names_trader;
int prevUserSize = 0;
int prevSize = 0;

void initialize_trade_line_stock_quantity(vector <int> &stock_quantity) {
    for ( int i = 0 ; i < stock_names_trader.size(); i ++ ) {
        stock_quantity.push_back(0);
    }
}

struct stock_info_trader{
    string companyName;
    ll companyIndex = -1;
    ll quantity;
};

struct trade_line {
    string brokerName;
    vector <stock_info_trader> equation_parameters;
    vector<int> stock_quantity;
    string action;
    int package_price;
    bool participating = true;
    int total_quantity;
    int start_time;
    int expiration_period;
};





vector <trade_line> all_trades_trader;
vector <string> all_company_names;

int maximum_individual_stock_quantity = 11;
int maximum_recursive_threshold = 5;







////////////////////////////////////////////


void init_string(string &s){
    s = "";
}

void init_stock_info_trader(stock_info_trader &info){
    info.companyName = "";
    info.quantity = 0;
}

void init_trade_line_trader(trade_line &line){
    line.action = "";
    line.equation_parameters.clear();
    line.stock_quantity.clear();
    line.package_price = 0;
}

void init_push_parameters(string &a, string &b, string &c, string &d){
    a = "";
    b = "";
    c = "";
    d = "";
}

void quantity_cycle_detect(vector<int> check_trade_lines, vector<int> current_quantity, int current_index, vector<int> &max_quantity_possible, int &max_amount_possible) {
    int sum;
    for (int i = 0 ; i < stock_names_trader.size() ; i ++) {
        sum = 0 ; 
        for (int j = 0 ; j < check_trade_lines.size() ; j ++) {
            if (all_trades_trader[check_trade_lines[j]].participating) {
                if (all_trades_trader[check_trade_lines[j]].action == "b") {
                    sum += all_trades_trader[check_trade_lines[j]].stock_quantity[i] * current_quantity[j];
                }
                else if (all_trades_trader[check_trade_lines[j]].action == "s") {
                    sum -= all_trades_trader[check_trade_lines[j]].stock_quantity[i] * current_quantity[j];
                }
            }
        }
        if (sum!=0) break;
    }
    if (sum == 0) {
        int total_sum = 0;
        
        for (int i = 0 ; i < check_trade_lines.size() ; i ++) {
            if (all_trades_trader[check_trade_lines[i]].participating) {
                if (all_trades_trader[check_trade_lines[i]].action == "b") {
                    total_sum += all_trades_trader[check_trade_lines[i]].package_price*current_quantity[i];
                }
                else if (all_trades_trader[check_trade_lines[i]].action == "s") {
                    total_sum -= all_trades_trader[check_trade_lines[i]].package_price*current_quantity[i];
                }
            }
        }
        if (total_sum > 0 && total_sum > max_amount_possible) {
            max_amount_possible = total_sum;
            max_quantity_possible = current_quantity;
        }
    }
    if (current_index != check_trade_lines.size()) {
        if (all_trades_trader[current_index].participating == true && current_quantity[current_index] < all_trades_trader[current_index].total_quantity && current_quantity[current_index] <= maximum_individual_stock_quantity) {
            for (int i = current_quantity[current_index] ; i < all_trades_trader[current_index].total_quantity && i <= maximum_individual_stock_quantity ; i ++) {
                current_quantity[current_index] ++;
                quantity_cycle_detect(check_trade_lines, current_quantity, current_index + 1, max_quantity_possible, max_amount_possible);
            }
        }
        else {
            quantity_cycle_detect(check_trade_lines, current_quantity, current_index + 1, max_quantity_possible, max_amount_possible);
        }
    }
}

void all_cycle_detector(vector<int> check_trade_lines, vector <pair <pair <vector<int>, vector<int> >, int> > &all_cyles_to_be_returned, int recursive_depth) {
    if (recursive_depth == maximum_recursive_threshold) return;
    vector<int> current_quantity;
    for (int i = 0 ; i < check_trade_lines.size() ; i ++) {
        current_quantity.push_back(1);
    }
    int max_amount_possible = 0;
    vector<int> max_quantity_possible;
    quantity_cycle_detect(check_trade_lines, current_quantity, 0, max_quantity_possible, max_amount_possible);
    if (max_quantity_possible.size() > 0) {
        int sum = 0;
        vector <int> trade_lines_to_be_appended;
        vector<int> quantities_to_be_appended;
        for (int i = 0 ; i < check_trade_lines.size() ; i ++) {
            if (all_trades_trader[check_trade_lines[i]].participating) {
                trade_lines_to_be_appended.push_back(i);
                quantities_to_be_appended.push_back(max_quantity_possible[i]);
            }
        }
        all_cyles_to_be_returned.push_back(make_pair(make_pair(trade_lines_to_be_appended, quantities_to_be_appended), max_amount_possible));
    }

    for (int i = 0 ; i < check_trade_lines.size()-1 ; i ++) {
        if (all_trades_trader[check_trade_lines[i]].participating) {
            all_trades_trader[check_trade_lines[i]].participating = false;
            all_cycle_detector(check_trade_lines, all_cyles_to_be_returned, recursive_depth + 1);
            all_trades_trader[check_trade_lines[i]].participating = true;
        }
    }
}

pair <pair <vector<int>, vector<int> >, int> max_cycle_detector(vector<int> check_trade_lines) {
    vector <pair <pair <vector<int>, vector<int> >, int> > all_cyles_to_be_returned;
    all_cycle_detector(check_trade_lines, all_cyles_to_be_returned, 0);
    pair <pair <vector<int>, vector<int> >, int> trade_lines_to_be_returned;
    if (all_cyles_to_be_returned.size() == 0) {
        return trade_lines_to_be_returned;
    }
    int max_index = 0;
    for (int i = 1 ; i < all_cyles_to_be_returned.size() ; i ++) {
        if (all_cyles_to_be_returned[i].second > all_cyles_to_be_returned[max_index].second) max_index = i;
    }
    return all_cyles_to_be_returned[max_index];
}

bool equation_parameter_matcher (vector<int> equation1, vector<int> equation2) {
    for (int i = 0 ; i < equation1.size() ; i ++ ) {
        if (equation1[i] != equation2[i]) {
            return false;
        }
    }
    return true;
}

//////////////////////////////////////
void initialize_bool_trader(bool &r1, bool &r2, bool &r3, bool &r4, bool &r5){
    r1 = false; r2 = false; r3 = false; r4 = false; r5 = false;
}

//////////////////////////////////////////////////////////////

vector<vector <int> > stock_entities;
vector<vector<int> > corresponding_prices;
vector<trade_line> all_user_trades;
string user_broker_name = "22B1003_22B0933";



void line_parser(string message) {
    bool start_time_in, broker_name_in, action_in, equation_in, line_done;
    initialize_bool_trader(start_time_in, broker_name_in, action_in, equation_in, line_done);

    stock_info_trader *push_info = new stock_info_trader;
    init_stock_info_trader(*push_info);
    trade_line *push_line = new trade_line;
    init_trade_line_trader(*push_line);

    ll message_len = message.size();
    
    string s1 = "";    

    ll i = 0;                        
    int num_spaces = 0;

    while(i < message_len) {
        if(message[i] == 33 && message[i+1] == 64){
            break;
        }
        else if(start_time_in == false){
            if(message[i] < 48 || message[i] > 57){
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
                    push_info->companyName = s1;
                    s1 = "";
                    num_spaces += 1;
                    i += 1;
                }
                i += 1;
            }
            else{
                if(message[i] == 36){
                    equation_in = true;
                    num_spaces = 0;
                    push_info->quantity = 1;
                    s1 = "";
                    push_line->equation_parameters.push_back(*push_info);
                }
                else if((message[i] < 65 || message [i] > 90) && (message[i] < 97 || message [i] > 122)){
                    s1 += message[i];
                    if(message[i+1] == 32){
                        push_info->quantity = stoi(s1);
                        s1 = "";
                        push_line->equation_parameters.push_back(*push_info);
                        init_stock_info_trader(*push_info);
                        num_spaces += 1;
                        i += 1;
                    }                    
                    i += 1;
                }
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
                    push_line->total_quantity = stoi(s1);
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
                    all_trades_trader.push_back(*push_line);
                    init_trade_line_trader(*push_line);
                    initialize_bool_trader(start_time_in, broker_name_in, action_in, equation_in, line_done);
                    s1 = "";
                    i += 1;
                    num_spaces = 0;
                }
                i += 1;
            }
        }
    }    

        
    for(int j = prevSize; j < all_trades_trader.size(); j++){
        for(int g = 0; g < (all_trades_trader[j]).equation_parameters.size(); g++){
            for(int z = 0; z < stock_names_trader.size(); z++){
                if(stock_names_trader[z] == (all_trades_trader[j]).equation_parameters[g].companyName){
                    (all_trades_trader[j]).equation_parameters[g].companyIndex = z;
                    break;
                }
            }
            if((all_trades_trader[j]).equation_parameters[g].companyIndex == -1){
                (all_trades_trader[j]).equation_parameters[g].companyIndex = stock_names_trader.size();
                stock_names_trader.push_back((all_trades_trader[j]).equation_parameters[g].companyName);
            }
        }
    }

    for(int j = prevSize; j < all_trades_trader.size(); j++){
        all_trades_trader[j].stock_quantity = vector<int>(stock_names_trader.size());
        for(int g = 0; g < (all_trades_trader[j]).equation_parameters.size(); g++){
            all_trades_trader[j].stock_quantity[(all_trades_trader[j]).equation_parameters[g].companyIndex] = (all_trades_trader[j]).equation_parameters[g].quantity;
        }
    }
}


bool found_linear_combo = false;
void all_trades_execute () {
        for (int i = prevSize ; i < all_trades_trader.size() ; i ++) {
        // check_trade_lines.push_back(i);
        bool found_this_equation = false;
        int found_position = corresponding_prices.size();
        for (int m = 0 ; m < stock_entities.size() ; m ++) {
            if (equation_parameter_matcher(all_trades_trader[i].stock_quantity, stock_entities[m])) {
                found_this_equation = true;
                found_position = m;
                // cout<<m<<"CHECK"<<endl;
                bool inserted = false;
                for (int y = 0 ; y < corresponding_prices[m].size() ; y ++ ) {
                    
                    if (corresponding_prices[m][y] < all_trades_trader[i].package_price) {
                        continue;
                    }
                    else {
                        corresponding_prices[m].insert(corresponding_prices[m].begin() + y, all_trades_trader[i].package_price);
                        inserted = true;
                        break;
                    }

                }
                if(!inserted) corresponding_prices[m].push_back(all_trades_trader[i].package_price);
                break;
            }
        }
        
        if (!found_this_equation) {
            vector<int> to_be_pushed = all_trades_trader[i].stock_quantity;
            stock_entities.push_back(to_be_pushed);
            vector<int> price_to_be_pushed;
            price_to_be_pushed.push_back(all_trades_trader[i].package_price);
            corresponding_prices.push_back(price_to_be_pushed);
        }

        if ( i == 0 ) {
            continue;
        }
        
        vector<int> check_trade_lines;
        for (int j = 0 ; j < i ; j ++) {
            if ((all_trades_trader[j].start_time + all_trades_trader[j].expiration_period < all_trades_trader[i].start_time && all_trades_trader[j].expiration_period != -1) ||all_trades_trader[j].total_quantity == 0) {
                all_trades_trader[j].participating = false;
            }
            if (all_trades_trader[j].equation_parameters.size() >= 2 ) found_linear_combo = true;
            if (all_trades_trader[j].participating) check_trade_lines.push_back(j);
        }

        check_trade_lines.push_back(i);
        
        if (found_linear_combo){
            // cout<<"WRONG"<<endl;
            pair <pair <vector<int>, vector<int> >, int> max_cycle_detected = max_cycle_detector(check_trade_lines);
            if (max_cycle_detected.first.first.size() > 0 ) {
                int minimum_expiration_time = INT16_MAX;
                int maximum_starting_time = 0;
                for (int j = 0 ; j < max_cycle_detected.first.first.size() ; j ++) {
                    if (all_trades_trader[max_cycle_detected.first.first[j]].start_time > maximum_starting_time) maximum_starting_time = all_trades_trader[max_cycle_detected.first.first[j]].start_time;
                    if (all_trades_trader[max_cycle_detected.first.first[j]].expiration_period == -1) continue;
                    if (all_trades_trader[max_cycle_detected.first.first[j]].expiration_period < minimum_expiration_time) minimum_expiration_time = all_trades_trader[max_cycle_detected.first.first[j]].expiration_period;
                }

                for (int j = 0 ; j < max_cycle_detected.first.first.size() ; j ++) {
                    trade_line user_trade;
                    user_trade.brokerName = user_broker_name;
                    if (all_trades_trader[max_cycle_detected.first.first[j]].action == "b") {
                        user_trade.action = "s";
                    }
                    else if (all_trades_trader[max_cycle_detected.first.first[j]].action == "s") {
                        user_trade.action = "b";
                    }
                    user_trade.equation_parameters = all_trades_trader[max_cycle_detected.first.first[j]].equation_parameters;
                    user_trade.package_price = all_trades_trader[max_cycle_detected.first.first[j]].package_price;
                    user_trade.stock_quantity = all_trades_trader[max_cycle_detected.first.first[j]].stock_quantity;
                    user_trade.total_quantity = max_cycle_detected.first.second[j];
                    user_trade.start_time = maximum_starting_time;
                    user_trade.expiration_period = minimum_expiration_time;
                    all_user_trades.push_back(user_trade);
                    if (all_trades_trader[max_cycle_detected.first.first[j]].total_quantity == user_trade.total_quantity) {
                        all_trades_trader[max_cycle_detected.first.first[j]].participating = false;
                    }
                    else {
                        all_trades_trader[max_cycle_detected.first.first[j]].total_quantity -= user_trade.total_quantity;
                    }
                }
                continue;
            }}
            
            // cout<<(corresponding_prices[found_position][corresponding_prices[found_position].size()/2] + corresponding_prices[found_position][corresponding_prices[found_position].size()/2])/2<<endl;
        
        if (all_trades_trader[i].action == "b" && corresponding_prices[found_position].size() > 1 && corresponding_prices[found_position].size()%2 == 0 && all_trades_trader[i].package_price > (corresponding_prices[found_position][corresponding_prices[found_position].size()/2 - 1] + corresponding_prices[found_position][corresponding_prices[found_position].size()/2])/2) {
            trade_line user_trade;
            user_trade.brokerName = user_broker_name;
            user_trade.action = "s";
            user_trade.equation_parameters = all_trades_trader[i].equation_parameters;
            user_trade.package_price = all_trades_trader[i].package_price;
            user_trade.start_time = all_trades_trader[i].start_time;
            // user_trade.expiration_period = all_trades_trader[i].expiration_period;
            user_trade.expiration_period = 1;
            user_trade.stock_quantity = all_trades_trader[i].stock_quantity;
            user_trade.total_quantity = all_trades_trader[i].total_quantity;
            all_user_trades.push_back(user_trade);
            all_trades_trader[i].participating = false;
        }

        else if (all_trades_trader[i].action == "b" && corresponding_prices[found_position].size() > 1 && corresponding_prices[found_position].size()%2 == 1 && all_trades_trader[i].package_price > corresponding_prices[found_position][corresponding_prices[found_position].size()/2]) {
            trade_line user_trade;
            user_trade.brokerName = user_broker_name;
            user_trade.action = "s";
            user_trade.equation_parameters = all_trades_trader[i].equation_parameters;
            user_trade.package_price = all_trades_trader[i].package_price;
            user_trade.start_time = all_trades_trader[i].start_time;
            user_trade.expiration_period = 1;
            // user_trade.expiration_period = all_trades_trader[i].expiration_period;
            user_trade.stock_quantity = all_trades_trader[i].stock_quantity;
            user_trade.total_quantity = all_trades_trader[i].total_quantity;
            all_user_trades.push_back(user_trade);
            all_trades_trader[i].participating = false;
        }

        else if (all_trades_trader[i].action == "s" && corresponding_prices[found_position].size() > 1 && corresponding_prices[found_position].size()%2 == 0 && all_trades_trader[i].package_price < (corresponding_prices[found_position][corresponding_prices[found_position].size()/2 - 1] + corresponding_prices[found_position][corresponding_prices[found_position].size()/2]) / 2) {
            trade_line user_trade;
            user_trade.brokerName = user_broker_name;
            user_trade.action = "b";
            user_trade.equation_parameters = all_trades_trader[i].equation_parameters;
            user_trade.package_price = all_trades_trader[i].package_price;
            user_trade.start_time = all_trades_trader[i].start_time;
            user_trade.expiration_period = 1;
            // user_trade.expiration_period = all_trades_trader[i].expiration_period;
            user_trade.stock_quantity = all_trades_trader[i].stock_quantity;
            user_trade.total_quantity = all_trades_trader[i].total_quantity;
            all_user_trades.push_back(user_trade);
            all_trades_trader[i].participating = false;
        }

        else if (all_trades_trader[i].action == "s" && corresponding_prices[found_position].size() > 1 && corresponding_prices[found_position].size()%2 == 1 && all_trades_trader[i].package_price < corresponding_prices[found_position][corresponding_prices[found_position].size()/2]) {
            trade_line user_trade;
            user_trade.brokerName = user_broker_name;
            user_trade.action = "b";
            user_trade.equation_parameters = all_trades_trader[i].equation_parameters;
            user_trade.package_price = all_trades_trader[i].package_price;
            user_trade.start_time = all_trades_trader[i].start_time;
            user_trade.expiration_period = 1;
            // user_trade.expiration_period = all_trades_trader[i].expiration_period;
            user_trade.stock_quantity = all_trades_trader[i].stock_quantity;
            user_trade.total_quantity = all_trades_trader[i].total_quantity;
            all_user_trades.push_back(user_trade);
            all_trades_trader[i].participating = false;
        }
    }
    prevSize = all_trades_trader.size();
}


////////////////////


int reader(int time)
{

    string filename = "output.txt";
    std::ifstream file(filename);
    int current_line_number = 0;
    while (true) {
        int line_number = 0 ;
        std::string line;
        while(std::getline(file, line)) {
            // 
            if (line_number >= current_line_number) {
                all_lines.push_back(line);

                ///////////////////
                

                if (line.find(user_broker_name) == std::string::npos) {
                    line_parser(line);
                }
                all_trades_execute();
                // cout<<all_user_trades.size()<<endl;
                // cout<<all_trades_trader.size()<<endl;
                int currentTime;
                {
                    currentTime = commonTimer.load();
                }
                std::lock_guard<std::mutex> lock(printMutex);

                for (int l = prevUserSize ; l < all_user_trades.size() ; l ++ ) {
                    string outputString  = "";
                    // 0 KarGoCorp SELL AMZN $100 #30 10
                    outputString += to_string(currentTime) +  " " + all_user_trades[l].brokerName + " ";
                    // cout<<all_user_trades[l].start_time<<" "<<all_user_trades[l].brokerName<<" ";
                    if (all_user_trades[l].action == "s") {
                        // cout<<"SELL ";
                        outputString += "SELL ";
                    }
                    else {
                        outputString += "BUY ";
                        // cout<<"BUY ";
                    }
                    if (all_user_trades[l].equation_parameters.size() == 1) {
                        outputString += all_user_trades[l].equation_parameters[0].companyName + " ";
                        // cout<<all_user_trades[l].equation_parameters[0].companyName<<" ";
                    }
                    else {
                        for (int q = 0 ; q < all_user_trades[l].equation_parameters.size() ; q ++ ){
                            outputString += all_user_trades[l].equation_parameters[q].companyName+ " " + to_string(all_user_trades[l].equation_parameters[q].quantity) + " " ;
                            // cout<<all_user_trades[l].equation_parameters[q].companyName<<" "<<all_user_trades[l].equation_parameters[q].quantity<<" ";
                        }
                    }
                    outputString += "$" + to_string(all_user_trades[l].package_price) + " #" + to_string(all_user_trades[l].total_quantity) + " " + to_string(all_user_trades[l].expiration_period);
                    // cout<<"$"<<all_user_trades[l].package_price<<" #"<<all_user_trades[l].total_quantity<<" "<<all_user_trades[l].expiration_period<<endl;
                    cout<<outputString<<endl;
                }
                prevUserSize = all_user_trades.size();
                // cout<<"Check"<<endl;
                // std::cout << currentTime << " " << "Saksham SELL AMZN $100 #30 10" << std::endl;
                // if (line_number == 5 ) {
                //     std::cout << currentTime << " " << "Saksham SELL AMZN $100 #30 10" << std::endl;
                // }
                // 
                
                // cout<<"HELLO WORLD" << endl;
            }
            line_number ++;
            if (line == "@!" || line == "Successfully Initiated!") {
                // cout<<"CHECK"<<endl;
                return 1;
            }
            line_number ++;
        }

        // 
        current_line_number = line_number;
        file.clear();
        file.seekg(0, std::ios::end);
        if (!found_linear_combo) {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
        // cout<<all_lines.size()<<endl;
        if (all_lines[all_lines.size() - 1] == "!@" || all_lines[all_lines.size() - 1] == "Successfully Initiated!" || all_lines[all_lines.size() - 1] == "@!")  { 
            // cout<<"CHECK"<<endl;
            break;
        }
    }
    
    return 1;
}

int trader(std::string *message)
{
    return 1;
}