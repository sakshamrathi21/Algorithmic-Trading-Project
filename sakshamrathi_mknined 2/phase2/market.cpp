#include "market.h"
#include<iostream>
#include<vector>
#include<string>
#include<cstring>
#include<fstream>
#define ll long long

using namespace std;

struct broker_end_of_day {
    string brokerName;
    int net_transfer = 0;
    int quantity_bought = 0;
    int quantity_sold = 0;
};

struct stock_info{
    string companyName;
    ll companyIndex = -1;
    ll quantity;
};

struct trade_line {
    string broker_name;
    vector<stock_info> equation_parameters;
    vector<int> stock_quantity;
    string action;
    int package_price;
    bool participating = true;
    int total_quantity;
    int start_time;
    int expiration_period;
};

struct order_executed {
    int buyer;
    int seller;
    int price;
    int quantity_exchanged;
    vector<stock_info> equation_parameters_order_completed;
};

vector<order_executed> all_orders_completed;

vector <string> stock_names;
vector <string> all_broker_names;
vector <trade_line> all_trades;

bool trade_line_match(int trade_line_1, int trade_line_2) {
    if (all_trades[trade_line_1].participating == false || all_trades[trade_line_2].participating == false) return false;
    if (all_trades[trade_line_1].action == all_trades[trade_line_2].action) return false;
    for (int i = 0 ; i < stock_names.size() ; i ++) {
        if (all_trades[trade_line_1].stock_quantity[i] != all_trades[trade_line_2].stock_quantity[i] ) return false;
    }
    return true;
}

void sort_based_on_alphabet(vector<int> &trade_lines_those_match, int start_location, int last_location, int current_trade_line){
    for (int i = start_location ; i <= last_location ; i ++) {
        if (all_trades[trade_lines_those_match[i]].broker_name == all_trades[current_trade_line].broker_name) {
            int temp = trade_lines_those_match[start_location];
            trade_lines_those_match[start_location] = trade_lines_those_match[i];
            trade_lines_those_match[i] = temp;
            start_location ++;
        }
    }
    for(int i = last_location; i > start_location; i--){
        int max_index = i;
        for(int g = start_location; g <= i; g++){
            if(all_trades[trade_lines_those_match[g]].broker_name > all_trades[trade_lines_those_match[i]].broker_name) max_index = g;
        }
        int temp = trade_lines_those_match[i];
        trade_lines_those_match[i] = trade_lines_those_match[max_index];
        trade_lines_those_match[max_index] = temp;
    }
}

void sort_based_on_time (vector<int> &trade_lines_those_match, int j, int last_location, int current_trade_line) {
    for(int i = last_location; i > j; i--){
        if(all_trades[trade_lines_those_match[i]].broker_name == "22B1003_22B0933"){
            int temp = trade_lines_those_match[j];
            trade_lines_those_match[j] = trade_lines_those_match[i];
            trade_lines_those_match[i] = temp;
            j++;
        }
    }
    for(int i = last_location; i > j; i--){
        int max_index = i;
        for(int g = j; g <= i; g++){
            if(all_trades[trade_lines_those_match[g]].start_time > all_trades[trade_lines_those_match[i]].start_time) max_index = i;
        }
        int temp = trade_lines_those_match[i];
        trade_lines_those_match[i] = trade_lines_those_match[max_index];
        trade_lines_those_match[max_index] = temp;
    }
    int tr = last_location;
    while(tr > j && tr != 1){
        int ending = tr;
        while(tr >= j && all_trades[trade_lines_those_match[tr]].start_time == all_trades[trade_lines_those_match[tr-1]].start_time) {
            if (tr == 1 ) break;
            tr -= 1;
        }
        sort_based_on_alphabet(trade_lines_those_match, tr, ending, current_trade_line);
    }
}

void increasing_sort_all_matches (vector<int> &trade_lines_those_match, int current_trade_line) {
    int n = trade_lines_those_match.size();
    for (int i = 1 ; i < n ; i ++) {
        int key = trade_lines_those_match[i];
        int j = i - 1;
        while (j >= 0 && all_trades[trade_lines_those_match[j]].package_price > all_trades[key].package_price) {
            trade_lines_those_match[j + 1] = trade_lines_those_match[j]; 
            j --;
        }
        trade_lines_those_match[j+1] = key;
        int last_location = j + 1;
        while (j >= 0 && all_trades[trade_lines_those_match[j]].package_price == all_trades[key].package_price) {
            j --;
        }
        j ++;
        sort_based_on_time(trade_lines_those_match, j, last_location, current_trade_line);
        
    }
}

void decreasing_sort_matches (vector<int> &trade_lines_those_match, int current_trade_line) {
    int n = trade_lines_those_match.size();
    for (int i = 1 ; i < n ; i ++) {
        int key = trade_lines_those_match[i];
        int j = i - 1;
        while (j >= 0 && all_trades[trade_lines_those_match[j]].package_price < all_trades[key].package_price) {
            trade_lines_those_match[j + 1] = trade_lines_those_match[j]; 
            j --;
        }
        trade_lines_those_match[j+1] = key;
        int last_location = j + 1;
        while (j >= 0 && all_trades[trade_lines_those_match[j]].package_price == all_trades[key].package_price) {
            j --;
        }
        j ++;
        sort_based_on_time(trade_lines_those_match, j, last_location, current_trade_line);
    }
}
//////////####################################################################
void initialize_bool(bool &r1, bool &r2, bool &r3, bool &r4, bool &r5){
    r1 = false; r2 = false; r3 = false; r4 = false; r5 = false;
}

void init_stock_info(stock_info &info){
    info.companyName = "";
    info.quantity = 0;
}

void init_trade_line(trade_line &line){
    line.action = "";
    line.equation_parameters.clear();
    line.stock_quantity.clear();
    line.package_price = 0;
}

market::market(int argc, char** argv)
{
	//cout<<argc<<endl;
    // if (argc!= 2) return;
    // cout<<argv[2]<<endl;
    // cout<<message<<endl;
}

void market::start()
{
	// STRING PARSING
    // STRING PARSING
    std::ifstream file("output.txt");
    // cout<<"HELLO WORLD"<<endl;
    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Unable to open the file!" << std::endl;
        return;
    }

    // Read the contents of the file into a string
    std::string message((std::istreambuf_iterator<char>(file)),
                        (std::istreambuf_iterator<char>()));

    // Close the file
    file.close();

    bool start_time_in, broker_name_in, action_in, equation_in, line_done;
    initialize_bool(start_time_in, broker_name_in, action_in, equation_in, line_done);

    stock_info *push_info = new stock_info;
    init_stock_info(*push_info);
    trade_line *push_line = new trade_line;
    init_trade_line(*push_line);

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

                bool broker_inserted = false;
                for(ll g = 0; g < all_broker_names.size(); g++){
                    if(s1 == all_broker_names[g]){
                        broker_inserted = true;
                        break;
                    }
                }
                if(broker_inserted == false) all_broker_names.push_back(s1);

                push_line -> broker_name = s1;
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
                        init_stock_info(*push_info);
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

        
    for(int j = 0; j < all_trades.size(); j++){
        for(int g = 0; g < (all_trades[j]).equation_parameters.size(); g++){
            for(int z = 0; z < stock_names.size(); z++){
                if(stock_names[z] == (all_trades[j]).equation_parameters[g].companyName){
                    (all_trades[j]).equation_parameters[g].companyIndex = z;
                    break;
                }
            }
            if((all_trades[j]).equation_parameters[g].companyIndex == -1){
                (all_trades[j]).equation_parameters[g].companyIndex = stock_names.size();
                stock_names.push_back((all_trades[j]).equation_parameters[g].companyName);
            }
        }
    }

    for(int j = 0; j < all_trades.size(); j++){
        all_trades[j].stock_quantity = vector<int>(stock_names.size());
        for(int g = 0; g < (all_trades[j]).equation_parameters.size(); g++){
            all_trades[j].stock_quantity[(all_trades[j]).equation_parameters[g].companyIndex] = (all_trades[j]).equation_parameters[g].quantity;
        }
    }

    


    for (int i = 1 ; i < all_trades.size() ; i ++) {

        for (int j = 0 ; j < i ; j ++) {
            if ((all_trades[j].start_time + all_trades[j].expiration_period < all_trades[i].start_time && all_trades[j].expiration_period != -1) ||all_trades[j].total_quantity == 0) {
                all_trades[j].participating = false;
            }
        }
        vector<bool> trade_line_matches_or_not;
        for (int j = 0 ; j < i ; j ++) {
            trade_line_matches_or_not.push_back(trade_line_match(j, i));
        }
        vector <int> lines_those_match;
        for (int j = 0 ; j < i ; j ++) {
            if (trade_line_matches_or_not[j]) {
                lines_those_match.push_back(j);
            }
        }
        if (lines_those_match.size() == 0) continue;
        if (all_trades[i].action == "b") {
            increasing_sort_all_matches(lines_those_match, i);
            int l = 0;
            while (l < lines_those_match.size()) {
                if (all_trades[i].total_quantity <= all_trades[lines_those_match[l]].total_quantity) {
                    if (all_trades[lines_those_match[l]].package_price > all_trades[i].package_price) break;
                    order_executed new_order_to_be_appended;
                    new_order_to_be_appended.buyer = i;
                    new_order_to_be_appended.seller = lines_those_match[l];
                    new_order_to_be_appended.price = all_trades[lines_those_match[l]].package_price;
                    new_order_to_be_appended.quantity_exchanged = all_trades[i].total_quantity;
                    new_order_to_be_appended.equation_parameters_order_completed = all_trades[i].equation_parameters;
                    all_orders_completed.push_back(new_order_to_be_appended);
                    all_trades[lines_those_match[l]].total_quantity -= all_trades[i].total_quantity;
                    all_trades[i].total_quantity = 0;
                    
                    
                    break;
                }
                else {
                    if (all_trades[lines_those_match[l]].package_price > all_trades[i].package_price) break;
                    order_executed new_order_to_be_appended;
                    new_order_to_be_appended.buyer = i;
                    new_order_to_be_appended.seller = lines_those_match[l];
                    new_order_to_be_appended.price = all_trades[lines_those_match[l]].package_price;
                    new_order_to_be_appended.quantity_exchanged = all_trades[lines_those_match[l]].total_quantity;
                    new_order_to_be_appended.equation_parameters_order_completed = all_trades[i].equation_parameters;
                    all_orders_completed.push_back(new_order_to_be_appended);

                    all_trades[i].total_quantity -= all_trades[lines_those_match[l]].total_quantity;
                    all_trades[lines_those_match[l]].total_quantity = 0;
                }
                l ++;
            }
        }
        else if (all_trades[i].action == "s") {
            decreasing_sort_matches(lines_those_match, i);
            int l = 0;
            while (l < lines_those_match.size()) {
                if (all_trades[i].total_quantity <= all_trades[lines_those_match[l]].total_quantity) {
                    if (all_trades[lines_those_match[l]].package_price < all_trades[i].package_price) break;
                    order_executed new_order_to_be_appended;
                    new_order_to_be_appended.seller = i;
                    new_order_to_be_appended.buyer = lines_those_match[l];
                    new_order_to_be_appended.price = all_trades[lines_those_match[l]].package_price;
                    new_order_to_be_appended.quantity_exchanged = all_trades[i].total_quantity;
                    new_order_to_be_appended.equation_parameters_order_completed = all_trades[i].equation_parameters;
                    all_orders_completed.push_back(new_order_to_be_appended);
                    all_trades[lines_those_match[l]].total_quantity -= all_trades[i].total_quantity;
                    all_trades[i].total_quantity = 0;
                    
                    break;
                }
                else {
                    if (all_trades[lines_those_match[l]].package_price < all_trades[i].package_price) break;
                    order_executed new_order_to_be_appended;
                    new_order_to_be_appended.seller = i;
                    new_order_to_be_appended.buyer = lines_those_match[l];
                    new_order_to_be_appended.price = all_trades[lines_those_match[l]].package_price;
                    new_order_to_be_appended.quantity_exchanged = all_trades[lines_those_match[l]].total_quantity;
                    new_order_to_be_appended.equation_parameters_order_completed = all_trades[i].equation_parameters;
                    all_orders_completed.push_back(new_order_to_be_appended);

                    all_trades[i].total_quantity -= all_trades[lines_those_match[l]].total_quantity;
                    all_trades[lines_those_match[l]].total_quantity = 0;
                }
                l ++;
            }
        }
    }
    int net_amount_transferred = 0;
    int total_shares_traded = 0;
    vector<broker_end_of_day> all_broker_statistics;
    for (int t = 0 ; t < all_orders_completed.size() ; t ++) {
        net_amount_transferred += all_orders_completed[t].price*all_orders_completed[t].quantity_exchanged;
        total_shares_traded += all_orders_completed[t].quantity_exchanged;
        bool found_buyer = false;
        for (auto &broker : all_broker_statistics) {
            if (all_trades[all_orders_completed[t].buyer].broker_name == broker.brokerName) {
                found_buyer = true;
                broker.net_transfer -= all_orders_completed[t].price*all_orders_completed[t].quantity_exchanged;
                broker.quantity_bought += all_orders_completed[t].quantity_exchanged;
            }
        }
        if (found_buyer == false) {
            broker_end_of_day broker;
            broker.net_transfer -= all_orders_completed[t].price*all_orders_completed[t].quantity_exchanged;
            broker.quantity_bought += all_orders_completed[t].quantity_exchanged;
            broker.brokerName = all_trades[all_orders_completed[t].buyer].broker_name;
            all_broker_statistics.push_back(broker);
        }

        bool found_seller = false;
        for (auto &broker : all_broker_statistics) {
            if (all_trades[all_orders_completed[t].seller].broker_name == broker.brokerName) {
                found_seller = true;
                broker.net_transfer += all_orders_completed[t].price*all_orders_completed[t].quantity_exchanged;
                broker.quantity_sold += all_orders_completed[t].quantity_exchanged;
            }
        }
        if (found_seller == false) {
            broker_end_of_day broker;
            broker.net_transfer += all_orders_completed[t].price*all_orders_completed[t].quantity_exchanged;
            broker.quantity_sold += all_orders_completed[t].quantity_exchanged;
            broker.brokerName = all_trades[all_orders_completed[t].seller].broker_name;
            all_broker_statistics.push_back(broker);
        }

        cout<<all_trades[all_orders_completed[t].buyer].broker_name<<" purchased "<<all_orders_completed[t].quantity_exchanged<<" share of ";
        if (all_orders_completed[t].equation_parameters_order_completed.size() == 1) {
            cout<<all_orders_completed[t].equation_parameters_order_completed[0].companyName<<" ";
        }
        else {
        for (int l = 0 ; l < all_orders_completed[t].equation_parameters_order_completed.size() ; l ++) {
            cout<<all_orders_completed[t].equation_parameters_order_completed[l].companyName<<" "<<all_orders_completed[t].equation_parameters_order_completed[l].quantity<<" ";
        }}
        cout<<"from "<<all_trades[all_orders_completed[t].seller].broker_name<<" for $"<<all_orders_completed[t].price<<"/share"<<endl;
    }
    cout<<endl;
    cout<<"---End of Day---"<<endl;
    cout<<"Total Amount of Money Transferred: $"<<net_amount_transferred<<endl;
    cout<<"Number of Completed Trades: "<<all_orders_completed.size()<<endl;
    cout<<"Number of Shares Traded: "<<total_shares_traded<<endl;
    for (auto broker : all_broker_statistics) {
        cout<<broker.brokerName<<" bought "<<broker.quantity_bought<<" and sold "<<broker.quantity_sold<<" for a net transfer of $"<<broker.net_transfer<<endl;
    }
    for (int i = 0 ; i < all_broker_names.size() ; i ++ ) {
        bool found = false;
        for (auto broker : all_broker_statistics) {
            if (broker.brokerName == all_broker_names[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout<<all_broker_names[i] << " bought " << 0 << " and sold " << 0 << " for a net transfer of $0"<<endl;
        }
    }
    ////////// All the trading brokers in all_broke_names /////////////////////////////
}