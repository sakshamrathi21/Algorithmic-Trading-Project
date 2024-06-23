#include<iostream>
#include<vector>
#include<string>
// #include "my_map.cpp"

#define ll int64_t

using namespace std;

vector <string> stock_names;

void initialize_trade_line_mapped_stock_quantity(vector <int> &mapped_stock_quantity) {
    for (int i = 0 ; i < stock_names.size(); i ++) {
        mapped_stock_quantity.push_back(0);
    }
}

struct stock_info{
    string companyName;
    ll companyIndex = -1;
    ll quantity;
};

struct trade_line {

    vector <stock_info> equation_parameters;
    vector<int> mapped_stock_quantity;
    // initialize_trade_line_mapped_stock_quantity(mapped_stock_quantity);
    string action;
    int package_price;
    bool participating = true;
};



vector <trade_line> all_trades;
vector <string> all_company_names;


////////////////////////////////////////////


void init_string(string &s){
    s = "";
}

void init_stock_info(stock_info &info){
    info.companyName = "";
    info.quantity = 0;
}

void init_trade_line(trade_line &line){
    line.action = "";
    line.equation_parameters.clear();
    line.mapped_stock_quantity.clear();
    line.package_price = 0;
}

void init_push_parameters(string &a, string &b, string &c, string &d){
    a = "";
    b = "";
    c = "";
    d = "";
}
////////////////////////////////////////////////

bool cycle_detect(vector<int> check_trade_lines) {
    for (int i = 0 ; i < stock_names.size() ; i ++) {
        int sum = 0 ; 
        for (int j = 0 ; j < check_trade_lines.size() ; j ++) {
            if (all_trades[check_trade_lines[j]].participating) {
                if (all_trades[check_trade_lines[j]].action == "b") {
                    sum += all_trades[check_trade_lines[j]].mapped_stock_quantity[i];
                }
                else if (all_trades[check_trade_lines[j]].action == "s") {
                    sum -= all_trades[check_trade_lines[j]].mapped_stock_quantity[i];
                }
            }
        }
        if (sum != 0) return false;
    }
    return true;
}

void all_cycle_detector(vector<int> check_trade_lines, vector <pair <vector<int>, int> > &all_cyles_to_be_returned) {
    if (cycle_detect(check_trade_lines)) {
        int sum = 0;
        vector <int> trade_lines_to_be_appended;
        for (int i = 0 ; i < check_trade_lines.size() ; i ++) {
            if (all_trades[check_trade_lines[i]].participating) {
                if (all_trades[check_trade_lines[i]].action == "b") {
                    sum += all_trades[check_trade_lines[i]].package_price;
                }
                else if (all_trades[check_trade_lines[i]].action == "s") {
                    sum -= all_trades[check_trade_lines[i]].package_price;
                }
                trade_lines_to_be_appended.push_back(i);
            }
        }
        if (sum > 0) all_cyles_to_be_returned.push_back(make_pair(trade_lines_to_be_appended, sum));
    }

    for (int i = 0 ; i < check_trade_lines.size()-1 ; i ++) {
        if (all_trades[check_trade_lines[i]].participating) {
            all_trades[check_trade_lines[i]].participating = false;
            all_cycle_detector(check_trade_lines, all_cyles_to_be_returned);
            all_trades[check_trade_lines[i]].participating = true;
        }
    }
}

vector<int> max_cycle_detector(vector<int> check_trade_lines) {
    vector <pair <vector<int>, int> > all_cyles_to_be_returned;
    all_cycle_detector(check_trade_lines, all_cyles_to_be_returned);
    vector<int> trade_lines_to_be_returned;
    if (all_cyles_to_be_returned.size() == 0) {
        return trade_lines_to_be_returned;
    }
    int max_index = 0;
    for (int i = 1 ; i < all_cyles_to_be_returned.size() ; i ++) {
        if (all_cyles_to_be_returned[i].second > all_cyles_to_be_returned[max_index].second) max_index = i;
    }
    return all_cyles_to_be_returned[max_index].first;
}



int main () {

    string message; 
    getline(cin, message);

    ll message_len = message.size();
    
    string name_traversal = "";               
    string quant_traversal = "";
    string action_traversal = "";
    string price_traversal = "";

    stock_info *push_info = new stock_info;
    init_stock_info(*push_info);

    trade_line *push_line = new trade_line;

    ll i = 0;                        
    int num_spaces = 0;
    while(i < message_len) {
        if(message[i] == 35){
            push_line->action = action_traversal;
            // if(action_traversal == "b"){
            //     push_line->action = "s";
            // }
            // else{
            //     push_line->action = "b";
            // }

            push_line->package_price = stoi(price_traversal);
            all_trades.push_back(*push_line);

            init_trade_line(*push_line);
            init_push_parameters(name_traversal, quant_traversal, action_traversal, price_traversal);
            i += 2;
            num_spaces = 0;
        }
        else if(message[i] == 32){
            if(num_spaces % 2 != 0){
                push_info->companyName = name_traversal;
                push_info->quantity = stoi(quant_traversal);
                push_line->equation_parameters.push_back(*push_info);
                init_string(name_traversal);
                init_string(quant_traversal);
            }
            num_spaces += 1;
            i += 1;
        }
        else if(num_spaces % 2 == 0){
            if((message[i] >= 65 && message [i] <= 90) || (message[i] >= 97 && message [i] <= 122)){
                name_traversal += message[i];
            }
            else{
                price_traversal += message[i];
            }
            i += 1;
        }
        else if(num_spaces % 2 != 0){
            if((message[i] < 65 || message [i] > 90) && (message[i] < 97 || message [i] > 122)){
                quant_traversal += message[i];
            }
            else{
                action_traversal += message[i];
            }
            i += 1;
        }
        
    }
    for(int j = 0; j < all_trades.size(); j++){
        for(int g = 0; g < (all_trades[j]).equation_parameters.size(); g++){
            for(int z = 0; z < all_company_names.size(); z++){
                if(all_company_names[z] == (all_trades[j]).equation_parameters[g].companyName){
                    (all_trades[j]).equation_parameters[g].companyIndex = z;
                    break;
                }
            }
            if((all_trades[j]).equation_parameters[g].companyIndex == -1){
                (all_trades[j]).equation_parameters[g].companyIndex = all_company_names.size();
                all_company_names.push_back((all_trades[j]).equation_parameters[g].companyName);
            }
        }
    }

    for(int j = 0; j < all_trades.size(); j++){
        all_trades[j].mapped_stock_quantity = vector<int>(all_company_names.size());
        for(int g = 0; g < (all_trades[j]).equation_parameters.size(); g++){
            all_trades[j].mapped_stock_quantity[(all_trades[j]).equation_parameters[g].companyIndex] = (all_trades[j]).equation_parameters[g].quantity;
        }
    }
    vector<int> all_trade_vec;
    for(int i = 0; i < all_trades.size(); i++) all_trade_vec.push_back(i);
    vector<int> max_cycle = max_cycle_detector(all_trade_vec);

for(int j = 0; j < max_cycle.size(); j++) cout<<max_cycle[j]<<" ";
}