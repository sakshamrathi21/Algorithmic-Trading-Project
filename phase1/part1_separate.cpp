#include<iostream>
#include<vector>
#include<string>
#include<fstream>

#define ll int64_t

using namespace std;

vector <string> stock_names;

void initialize_trade_line_stock_quantity(vector <int> &stock_quantity) {
    for (int i = 0 ; i < stock_names.size(); i ++) {
        stock_quantity.push_back(0);
    }
}

struct stock_info{
    string companyName;
    ll companyIndex = -1;
    ll quantity;
};

struct trade_line {

    vector <stock_info> equation_parameters;
    vector<int> stock_quantity;
    // initialize_trade_line_stock_quantity(stock_quantity);
    string action;
    int package_price;
    bool participating = true;
    int total_quantity;
};





vector <trade_line> all_trades;
vector <string> all_company_names;


////////////////////////////////////////////

void same_structure_cancelation(int i, int j) {
    if (all_trades[i].participating == false || all_trades[j].participating == false) return;
    if (all_trades[i].action == all_trades[j].action) return;
    if (all_trades[i].package_price != all_trades[j].package_price) return;
    for (int k = 0 ; k < stock_names.size() ; k ++) {
        if (all_trades[i].stock_quantity[k] != all_trades[i].stock_quantity[k]) return;
    }
    if (all_trades[i].total_quantity > all_trades[j].total_quantity) {
        all_trades[j].participating = false;
        all_trades[i].total_quantity -= all_trades[j].total_quantity;
        all_trades[j].total_quantity = 0;
    } 
    else if(all_trades[i].total_quantity < all_trades[j].total_quantity) {
        all_trades[i].participating = false;
        all_trades[j].total_quantity -= all_trades[i].total_quantity;
        all_trades[i].total_quantity = 0;
    }
    else {
        all_trades[i].total_quantity = 0;
        all_trades[j].total_quantity = 0;
        all_trades[i].participating = false;
        all_trades[j].participating = false;
    }
    return;
}


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
    for (int i = 0 ; i < stock_names.size() ; i ++) {
        sum = 0 ; 
        for (int j = 0 ; j < check_trade_lines.size() ; j ++) {
            if (all_trades[check_trade_lines[j]].participating) {
                if (all_trades[check_trade_lines[j]].action == "b") {
                    sum += all_trades[check_trade_lines[j]].stock_quantity[i] * current_quantity[j];
                }
                else if (all_trades[check_trade_lines[j]].action == "s") {
                    sum -= all_trades[check_trade_lines[j]].stock_quantity[i] * current_quantity[j];
                }
            }
        }
        if (sum!=0) break;
    }
    if (sum == 0) {
        int total_sum = 0;
        
        for (int i = 0 ; i < check_trade_lines.size() ; i ++) {
            if (all_trades[check_trade_lines[i]].participating) {
                if (all_trades[check_trade_lines[i]].action == "b") {
                    total_sum += all_trades[check_trade_lines[i]].package_price*current_quantity[i];
                }
                else if (all_trades[check_trade_lines[i]].action == "s") {
                    total_sum -= all_trades[check_trade_lines[i]].package_price*current_quantity[i];
                }
            }
        }
        if (total_sum > 0 && total_sum > max_amount_possible) {
            max_amount_possible = total_sum;
            max_quantity_possible = current_quantity;
        }
    }
    if (current_index != check_trade_lines.size()) {
        if (all_trades[current_index].participating == true && current_quantity[current_index] < all_trades[current_index].total_quantity) {
            for (int i = current_quantity[current_index] ; i < all_trades[current_index].total_quantity ; i ++) {
                current_quantity[current_index] ++;
                quantity_cycle_detect(check_trade_lines, current_quantity, current_index + 1, max_quantity_possible, max_amount_possible);
            }
        }
        else {
            quantity_cycle_detect(check_trade_lines, current_quantity, current_index + 1, max_quantity_possible, max_amount_possible);
        }
    }
}

void all_cycle_detector(vector<int> check_trade_lines, vector <pair <pair <vector<int>, vector<int> >, int> > &all_cyles_to_be_returned) {
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
            if (all_trades[check_trade_lines[i]].participating) {
                // if (all_trades[check_trade_lines[i]].action == "b") {
                //     sum += all_trades[check_trade_lines[i]].package_price;
                // }
                // else if (all_trades[check_trade_lines[i]].action == "s") {
                //     sum -= all_trades[check_trade_lines[i]].package_price;
                // }
                trade_lines_to_be_appended.push_back(i);
                quantities_to_be_appended.push_back(max_quantity_possible[i]);
            }
        }
        all_cyles_to_be_returned.push_back(make_pair(make_pair(trade_lines_to_be_appended, quantities_to_be_appended), max_amount_possible));
    }

    for (int i = 0 ; i < check_trade_lines.size()-1 ; i ++) {
        if (all_trades[check_trade_lines[i]].participating) {
            all_trades[check_trade_lines[i]].participating = false;
            all_cycle_detector(check_trade_lines, all_cyles_to_be_returned);
            all_trades[check_trade_lines[i]].participating = true;
        }
    }
}

pair <pair <vector<int>, vector<int> >, int> max_cycle_detector(vector<int> check_trade_lines) {
    vector <pair <pair <vector<int>, vector<int> >, int> > all_cyles_to_be_returned;
    all_cycle_detector(check_trade_lines, all_cyles_to_be_returned);
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


#define MAX_STOCKS 10000


struct stock_data{
    string companyName;
    string price;
    string actionOtherSide;
    int best_buy_price;
    int best_sell_price;
    bool buy_active = false;
    bool sell_active = false;
};

void initialize_stock_data(stock_data &curr_stock){
    curr_stock.actionOtherSide = "";
    curr_stock.price = "";
    curr_stock.companyName = "";
}

int main () {
    std::ifstream file("testcases/phase1/part1/input_long.txt");

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Unable to open the file!" << std::endl;
        return 1;
    }

    // Read the contents of the file into a string
    std::string message((std::istreambuf_iterator<char>(file)),
                        (std::istreambuf_iterator<char>()));

    // Close the file
    file.close();


    ll message_len = message.size();
    
    

        
        vector<stock_data> all_company_stocks;
        stock_data curr_stock;
        // intialize_stock_data(curr_stock);   
        string my_stocks[MAX_STOCKS];
        int last_trading_price[MAX_STOCKS];
        string last_traded_action[MAX_STOCKS];
        // int current_index = -1;
        string token;
        string stock;
        
        bool action;
        int price;
        ll current_index = -1;
        
   
            //if (message.size() == 0) continue;
            
            ll i = 0;                        // iterates over the entire message
        int num_spaces = 0;

        while(i < message_len) {
            if(message[i] == 35){
                i += 2;
                num_spaces = 0;
                bool found = false;
                for (int j = 0 ; j < all_company_stocks.size() ; j ++ ) {
                    // stock_data all_company_stocks[j] = all_company_stocks[j];
                    if (all_company_stocks[j].companyName == curr_stock.companyName) {
                        found = true;
                        if (all_company_stocks[j].buy_active) {
                            if (curr_stock.actionOtherSide == "s" && all_company_stocks[j].best_buy_price == stoi(curr_stock.price)) {
                                all_company_stocks[j].buy_active = false;
                                cout<<"No Trade"<<endl;
                                break;
                            }
                            else if (curr_stock.actionOtherSide == "b" && all_company_stocks[j].best_buy_price >= stoi(curr_stock.price)) {
                                cout<<"No Trade"<<endl;
                                break;
                            }
                            else if (curr_stock.actionOtherSide == "b") {
                                all_company_stocks[j].best_buy_price = stoi(curr_stock.price);
                            }
                        }
                        if (all_company_stocks[j].sell_active) {
                            found = true;
                            if (curr_stock.actionOtherSide == "b" && all_company_stocks[j].best_sell_price == stoi(curr_stock.price)) {
                                all_company_stocks[j].sell_active = false;
                                cout<<"No Trade"<<endl;
                                break;
                            }
                            else if (curr_stock.actionOtherSide == "s" && all_company_stocks[j].best_sell_price <= stoi(curr_stock.price)) {
                                cout<<"No Trade"<<endl;
                                break;
                            }
                            else if (curr_stock.actionOtherSide == "s") {
                                all_company_stocks[j].best_sell_price = stoi(curr_stock.price);
                            }
                        }


                        if (last_traded_action[j] == "s") {
                            // cout<<"BCJBJ"<<endl;
                            if (curr_stock.actionOtherSide == "s") {
                                if (stoi(curr_stock.price) < last_trading_price[j]) {
                                    // cout<<"CH"<<endl;
                                    last_trading_price[j] = stoi(curr_stock.price);
                                    last_traded_action[j] = "b";
                                    cout<<my_stocks[j]<<" "<<curr_stock.price<<" "<<"b"<<endl;
                                    break;
                                }
                                else {
                                    if (!all_company_stocks[j].sell_active) {
                                        all_company_stocks[j].sell_active = true;
                                        all_company_stocks[j].best_sell_price = stoi(curr_stock.price);
                                    }
                                    cout<<"No Trade"<<endl;
                                    break;
                                }
                            }
                            else if (curr_stock.actionOtherSide == "b") {
                                if (stoi(curr_stock.price) > last_trading_price[j]) {
                                    // cout<<"CH1"<<endl;
                                    last_trading_price[j] = stoi(curr_stock.price);
                                    last_traded_action[j] = "s";
                                    cout<<my_stocks[j]<<" "<<curr_stock.price<<" "<<"s"<<endl;
                                    break;
                                }
                                else {
                                    if (!all_company_stocks[j].buy_active) {
                                        all_company_stocks[j].buy_active = true;
                                        all_company_stocks[j].best_buy_price = stoi(curr_stock.price);
                                    }
                                    cout<<"No Trade"<<endl;
                                    break;
                                }
                            }
                        }
                        if (last_traded_action[j] == "b") {
                            // cout<<"CHECK"<<endl;
                            if (curr_stock.actionOtherSide == "s") {
                                // cout<<"HELLO"<<endl;
                                if (stoi(curr_stock.price) < last_trading_price[j]) {
                                    last_trading_price[j] = stoi(curr_stock.price);
                                    last_traded_action[j] = "b";
                                    cout<<my_stocks[j]<<" "<<curr_stock.price<<" "<<"b"<<endl;
                                    break;
                                }
                                else {
                                    if (!all_company_stocks[j].sell_active) {
                                        all_company_stocks[j].sell_active = true;
                                        all_company_stocks[j].best_sell_price = stoi(curr_stock.price);
                                    }
                                    cout<<"No Trade"<<endl;
                                    break;
                                }
                            }
                            else if (curr_stock.actionOtherSide == "b") {
                                if (stoi(curr_stock.price) > last_trading_price[j]) {
                                    // cout<<"CH3"<<endl;
                                    last_trading_price[j] = stoi(curr_stock.price);
                                    last_traded_action[j] = "s";
                                    cout<<my_stocks[j]<<" "<<curr_stock.price<<" "<<"s"<<endl;
                                    break;
                                }
                                else {
                                    if (!all_company_stocks[j].buy_active) {
                                        all_company_stocks[j].buy_active = true;
                                        all_company_stocks[j].best_buy_price = stoi(curr_stock.price);
                                    }
                                    cout<<"No Trade"<<endl;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
                if (!found) {
                    // cout<<"HELLO"<<endl;
                    current_index ++;
                    // cout<<current_index<<endl;
                    my_stocks[current_index] = curr_stock.companyName;
                    last_trading_price[current_index] = stoi(curr_stock.price);
                    all_company_stocks.push_back(curr_stock);
                    string action_print;
                    if (curr_stock.actionOtherSide == "s") {
                        action_print = "b";
                    }
                    else if (curr_stock.actionOtherSide == "b") {
                        action_print = "s";
                    }
                    else {
                        return 1;
                    }
                    last_traded_action[current_index] = action_print;
                    std::cout<<curr_stock.companyName<<" "<<curr_stock.price<<" "<<action_print<<endl;
                }
                initialize_stock_data(curr_stock);

            }
            else if(message[i] == 32){
                num_spaces += 1;
                i += 1;
            }
            else if(num_spaces == 0){
                curr_stock.companyName += message[i];
                i += 1;
            }
            else if(num_spaces == 1){
                curr_stock.price += message[i];
                i += 1;
            }
            else if(num_spaces == 2){
                curr_stock.actionOtherSide += message[i];
                i += 1;
            }
        }
//Same code as in trader .cpp but in a separate file. Reads from given_input.txt and outputs in the terminal.
        
}