#include "receiver.h"
#include<vector>
using namespace std;

#define ll long long
#define MAX_STOCKS 10000

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


vector <string> stock_names2;

void initialize_trade_line_mapped_stock_quantity2(vector <int> &mapped_stock_quantity) {
    for (int i = 0 ; i < stock_names2.size(); i ++) {
        mapped_stock_quantity.push_back(0);
    }
}

struct stock_info2{
    string companyName;
    ll companyIndex = -1;
    ll quantity;
};

struct trade_line2 {

    vector <stock_info2> equation_parameters;
    vector<int> mapped_stock_quantity;
    // initialize_trade_line_mapped_stock_quantity2(mapped_stock_quantity);
    string action;
    int package_price;
    bool participating = true;
};



vector <trade_line2> all_trades2;
vector <string> all_company_names2;


////////////////////////////////////////////


void init_string2(string &s){
    s = "";
}

void init_stock_info2(stock_info2 &info){
    info.companyName = "";
    info.quantity = 0;
}

void init_trade_line2(trade_line2 &line){
    line.action = "";
    line.equation_parameters.clear();
    line.mapped_stock_quantity.clear();
    line.package_price = 0;
}

void init_push_parameters2(string &a, string &b, string &c, string &d){
    a = "";
    b = "";
    c = "";
    d = "";
}
////////////////////////////////////////////////

bool same_trade_line_detector2(int i, int j) {
    // if (all_trades2[i].action != all_trades2[j].action) return false;
    for (int k = 0 ; k < stock_names2.size() ; k ++ ) {
        if (all_trades2[i].mapped_stock_quantity[k] != all_trades2[j].mapped_stock_quantity[k]) return false;
    }
    return true;
}

bool cycle_detect2(vector<int> check_trade_lines) {
    for (int i = 0 ; i < stock_names2.size() ; i ++) {
        int sum = 0 ; 
        for (int j = 0 ; j < check_trade_lines.size() ; j ++) {
            if (all_trades2[check_trade_lines[j]].participating) {
                if (all_trades2[check_trade_lines[j]].action == "b") {
                    sum += all_trades2[check_trade_lines[j]].mapped_stock_quantity[i];
                }
                else if (all_trades2[check_trade_lines[j]].action == "s") {
                    sum -= all_trades2[check_trade_lines[j]].mapped_stock_quantity[i];
                }
            }
        }
        if (sum != 0) return false;
    }
    return true;
}

void all_cycle_detector2(vector<int> check_trade_lines, vector <pair <vector<int>, int> > &all_cyles_to_be_returned) {
    if (cycle_detect2(check_trade_lines)) {
        int sum = 0;
        vector <int> trade_lines_to_be_appended;
        for (int i = 0 ; i < check_trade_lines.size() ; i ++) {
            if (all_trades2[check_trade_lines[i]].participating) {
                if (all_trades2[check_trade_lines[i]].action == "b") {
                    sum += all_trades2[check_trade_lines[i]].package_price;
                }
                else if (all_trades2[check_trade_lines[i]].action == "s") {
                    sum -= all_trades2[check_trade_lines[i]].package_price;
                }
                trade_lines_to_be_appended.push_back(i);
            }
        }
        if (sum > 0) all_cyles_to_be_returned.push_back(make_pair(trade_lines_to_be_appended, sum));
    }

    for (int i = 0 ; i < check_trade_lines.size()-1 ; i ++) {
        if (all_trades2[check_trade_lines[i]].participating) {
            all_trades2[check_trade_lines[i]].participating = false;
            all_cycle_detector2(check_trade_lines, all_cyles_to_be_returned);
            all_trades2[check_trade_lines[i]].participating = true;
        }
    }
}

pair <vector<int>, int> max_cycle_detector2(vector<int> check_trade_lines) {
    vector <pair <vector<int>, int> > all_cyles_to_be_returned;
    all_cycle_detector2(check_trade_lines, all_cyles_to_be_returned);
    pair <vector<int>, int> trade_lines_to_be_returned;
    if (all_cyles_to_be_returned.size() == 0) {
        return trade_lines_to_be_returned;
    }
    int max_index = 0;
    for (int i = 1 ; i < all_cyles_to_be_returned.size() ; i ++) {
        if (all_cyles_to_be_returned[i].second > all_cyles_to_be_returned[max_index].second) max_index = i;
    }
    return all_cyles_to_be_returned[max_index];
}


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

int main(int argc, char* argv[]) {

    

    if (argc!=2) {
        return 1;
    }
    if (*argv[1] == '1') {
        Receiver rcv;
        // sleep(5);
        std::string message = "";
        // while (true) {
        //     message += rcv.readIML();
        //     if (message.size() == 0) continue;
            
        // }
        

        
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
        while (true) {
            message = rcv.readIML();
            if (message.size() == 0) continue;
            ll message_len = message.size();
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

        if (message[message.size()-1] == '$') {
                break;
        }
        }
        rcv.terminate();
        // ll curr_stock_start_index = 0;    // stores the starting index of the current stock 
        
    }
    else if (*argv[1] == '2') {
    vector<int> all_trade_vec;
    int previous_size = 0 ;
    int all_names_prevSize2 = 0;
    Receiver rcv;
    // sleep(5);
    std::string message = "";
    
    
    int total_profit = 0;
    while (true) {
        message = "";
        message = rcv.readIML();
        //cout<<"///"<<message<<"/// "<<endl;
        if (message.size() == 0) continue;
        ll message_len = message.size();

        
        string name_traversal = "";               
        string quant_traversal = "";
        string action_traversal = "";
        string price_traversal = "";

        stock_info2 *push_info = new stock_info2;
        init_stock_info2(*push_info);

        trade_line2 *push_line = new trade_line2;

        ll i = 0;                        
        int num_spaces = 0;
        
        while(i < message_len) {
            if(message[i] == 35){
                push_line->action = action_traversal;

                push_line->package_price = stoi(price_traversal);
                all_trades2.push_back(*push_line);

                init_trade_line2(*push_line);
                init_push_parameters2(name_traversal, quant_traversal, action_traversal, price_traversal);
                i += 2;
                num_spaces = 0;
            }
            else if(message[i] == 32){
                if(num_spaces % 2 != 0){
                    push_info->companyName = name_traversal;
                    push_info->quantity = stoi(quant_traversal);
                    push_line->equation_parameters.push_back(*push_info);
                    init_string2(name_traversal);
                    init_string2(quant_traversal);
                }
                num_spaces += 1;
                i += 1;
            }
            else if(num_spaces % 2 == 0){
                if((message[i] >= 65 && message[i] <= 90) || (message[i] >= 97 && message[i] <= 122)){
                    name_traversal += message[i];
                }
                else{
                    price_traversal += message[i];
                }
                i += 1;
            }
            else if(num_spaces % 2 != 0){
                if((message[i] < 65 || message[i] > 90) && (message[i] < 97 || message[i] > 122)){
                    quant_traversal += message[i];
                }
                else{
                    action_traversal += message[i];
                }
                i += 1;
            }
            
        }

        for(int j = previous_size; j < all_trades2.size(); j++){
            for(int g = 0; g < (all_trades2[j]).equation_parameters.size(); g++){
                for(int z = 0; z < all_company_names2.size(); z++){
                    if(all_company_names2[z] == (all_trades2[j]).equation_parameters[g].companyName){
                        (all_trades2[j]).equation_parameters[g].companyIndex = z;
                        break;
                    }
                }
                if((all_trades2[j]).equation_parameters[g].companyIndex == -1){
                    (all_trades2[j]).equation_parameters[g].companyIndex = all_company_names2.size();
                    all_company_names2.push_back((all_trades2[j]).equation_parameters[g].companyName);
                }
            }
        }
        for(int j = 0; j < previous_size; j++){
            for(int g = all_names_prevSize2; g < all_company_names2.size(); g++){
                all_trades2[j].mapped_stock_quantity.push_back(0);
            }
        }
        for(int j = previous_size; j < all_trades2.size(); j++){
            all_trades2[j].mapped_stock_quantity = vector<int>(all_company_names2.size());
            for(int g = 0; g < (all_trades2[j]).equation_parameters.size(); g++){
                all_trades2[j].mapped_stock_quantity[(all_trades2[j]).equation_parameters[g].companyIndex] = (all_trades2[j]).equation_parameters[g].quantity;
            }
        }
        stock_names2 = all_company_names2;
        all_names_prevSize2 = all_company_names2.size();

        // if(all_trades2.size() == 6){
        //     for(int j = 0; j < 6;j++){
        //         cout<<"////";
        //         for(int i = 0; i < all_trades2[j].mapped_stock_quantity.size(); i++){
        //             cout<<all_trades2[j].mapped_stock_quantity[i]<<" ";
        //         }
        //         cout<<"////"<<endl;
        //     }
        // }
        
        
        for (int j = previous_size ; j < all_trades2.size() ; j ++ ) {
            all_trade_vec.push_back(j);
            if (j == 0) {
                cout<<"No Trade"<<endl;
                continue;
            }
            // cout<<"Hello World"<<endl;
            for (int k = 0 ; k < j ; k ++) {
                // cout<<"Hello World "<<j<<endl;
                if (all_trades2[j].action == "b") {
                    // cout<<"CHECK "<<j<<k<<endl;
                    // cout<<all_trades[k].action<<all_trades[j].action<<endl;
                    if (all_trades2[k].participating && same_trade_line_detector2(j, k) && all_trades2[k].action == all_trades2[j].action) {
                        // cout<<"Hello World"<<endl;
                        if (all_trades2[j].package_price >= all_trades2[k].package_price) all_trades2[k].participating = false;
                        else {all_trades2[j].participating = false; cout<<"No Trade"<<endl;}
                        break;
                    }
                    else {
                        vector<int> pair_checker;
                        pair_checker.push_back(k);
                        pair_checker.push_back(j);
                        if (all_trades2[k].participating && same_trade_line_detector2(j, k) && all_trades2[k].package_price==all_trades2[j].package_price && cycle_detect2(pair_checker)) {
                            all_trades2[k].participating = false;
                            all_trades2[j].participating = false;
                            cout<<"No Trade"<<endl;
                            break;
                        }
                        else {continue;}
                    }
                }
                else if (all_trades2[j].action == "s") {
                    if (all_trades2[k].participating && same_trade_line_detector2(j, k) && all_trades2[k].action == all_trades2[j].action) {
                        if (all_trades2[j].package_price <= all_trades2[k].package_price) all_trades2[k].participating = false;
                        else {all_trades2[j].participating = false; cout<<"No Trade"<<endl;}
                        break;
                    }
                    else {
                        vector<int> pair_checker;
                        pair_checker.push_back(k);
                        pair_checker.push_back(j);
                        if (all_trades2[k].participating && same_trade_line_detector2(j, k) && all_trades2[k].package_price==all_trades2[j].package_price && cycle_detect2(pair_checker)) {
                            all_trades2[k].participating = false;
                            all_trades2[j].participating = false;
                            cout<<"No Trade"<<endl;
                            break;
                        }
                        else {continue;}
                    }
                }
            }
            
            
            if (all_trades2[j].participating) {
                pair <vector<int>, int> max_cycle_found = max_cycle_detector2(all_trade_vec);
                if (max_cycle_found.first.size()!= 0) {
                    total_profit += max_cycle_found.second;
                    for (int t = 0 ; t < max_cycle_found.first.size(); t ++) {
                        all_trades2[max_cycle_found.first[t]].participating = false;
                    }
                    for (int n = max_cycle_found.first.size() - 1 ; n>= 0; n --) {
                        for (int t = 0 ; t < all_trades2[max_cycle_found.first[n]].equation_parameters.size() ; t ++) {
                            cout<<all_trades2[max_cycle_found.first[n]].equation_parameters[t].companyName<<" "<<all_trades2[max_cycle_found.first[n]].equation_parameters[t].quantity<<" ";
                        }
                        cout<<all_trades2[max_cycle_found.first[n]].package_price<<" ";
                        if (all_trades2[max_cycle_found.first[n]].action == "b") {
                            cout<<"s"<<endl;
                        }
                        else {
                            cout<<"b"<<endl;
                        }
                    }
                }
                else {
                    cout<<"No Trade"<<endl;
                }
            }
        }
        if (message[message.size()-1] == '$') {
            break;
        }
        previous_size = all_trades2.size();
        message = "";
    }
    rcv.terminate();

    cout<<total_profit<<endl;
    }
    

    else if (*argv[1] == '3') {
        int total_profit = 0;
        vector<int> check_trade_lines;
        int previous_size = 0 ; 
        int all_names_prevSize = 0;
        Receiver rcv;
        std::string message = "";
    
    while(true) {
        message = rcv.readIML();
        if (message.size() == 0) continue;
    
    string name_traversal = "";               
    string quant_traversal = "";
    string action_traversal = "";
    string price_traversal = "";
    string equation_count = "";

    stock_info *push_info = new stock_info;
    init_stock_info(*push_info);

    trade_line *push_line = new trade_line;

    ll i = 0;                        
    int num_spaces = 0;
    bool price_in = false, quantity_in = false;
    ll message_len = message.size();
    while(i < message_len) {
        if(message[i] == 35){
            push_line->action = action_traversal;
            push_line->total_quantity = stoi(equation_count);
            push_line->package_price = stoi(price_traversal);
            all_trades.push_back(*push_line);

            init_trade_line(*push_line);
            init_push_parameters(name_traversal, quant_traversal, action_traversal, price_traversal);
            init_string(equation_count);
            price_in = false;
            quantity_in = false;
            i += 2;
            num_spaces = 0;
        }
        else if(message[i] == 32){
            if(num_spaces % 2 != 0 && quantity_in == false){
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
            if(price_in == true && quantity_in == true){
                action_traversal += message[i];
                i += 1;
                continue;
            }
            if((message[i] >= 65 && message [i] <= 90) || (message[i] >= 97 && message [i] <= 122)){
                name_traversal += message[i];
            }
            else{
                price_traversal += message[i];
                if(message[i+1] == 32) price_in = true;
            }
            i += 1;
        }
        else if(num_spaces % 2 != 0){
            if((message[i] < 65 || message [i] > 90) && (message[i] < 97 || message [i] > 122)){
                if(price_in == true){
                    equation_count += message[i];
                    if(message[i+1] == 32) quantity_in = true;
                    i += 1;
                    continue;
                } 
                quant_traversal += message[i];
            }
            i += 1;
        }
        
    }
   
    for(int j = previous_size; j < all_trades.size(); j++){
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

    for(int j = 0; j < previous_size; j++){
        for(int g = all_names_prevSize; g < all_company_names.size(); g++){
            all_trades[j].stock_quantity.push_back(0);
        }
    }

    for(int j = previous_size; j < all_trades.size(); j++){
        all_trades[j].stock_quantity = vector<int>(all_company_names.size());
        for(int g = 0; g < (all_trades[j]).equation_parameters.size(); g++){
            all_trades[j].stock_quantity[(all_trades[j]).equation_parameters[g].companyIndex] = (all_trades[j]).equation_parameters[g].quantity;
        }
    }

    stock_names = all_company_names;
    
    all_names_prevSize = all_company_names.size();
    
    
    // check_trade_lines.push_back(0);
    for (int i = previous_size ; i < all_trades.size() ; i ++) {
        check_trade_lines.push_back(i);
        if (i ==0 ) {
            cout<<"No Trade"<<endl;
            continue;
        }

        for (int j = 0 ; j < i ; j ++) {
            same_structure_cancelation(i, j);
        }

        pair <pair <vector<int>, vector<int> >, int> max_cycle_detected = max_cycle_detector(check_trade_lines);
        if (max_cycle_detected.first.first.size() != 0) {
            for (int t = 0; t < max_cycle_detected.first.first.size() ; t ++) {
                all_trades[max_cycle_detected.first.first[t]].total_quantity -= max_cycle_detected.first.second[t];
                if (all_trades[max_cycle_detected.first.first[t]].total_quantity == 0) {
                    all_trades[max_cycle_detected.first.first[t]].participating = false;
                }
            }

            for (int l = max_cycle_detected.first.first.size() - 1 ; l >= 0 ; l --) {
                for (int n = 0 ; n < all_trades[max_cycle_detected.first.first[l]].equation_parameters.size() ; n ++) {
                    cout<<all_trades[max_cycle_detected.first.first[l]].equation_parameters[n].companyName<<" "<<all_trades[max_cycle_detected.first.first[l]].equation_parameters[n].quantity<<" ";
                }
                cout<<all_trades[max_cycle_detected.first.first[l]].package_price<<" ";
                cout<<max_cycle_detected.first.second[l]<<" ";
                if (all_trades[max_cycle_detected.first.first[l]].action == "b") {
                    cout<<"s"<<endl;
                }
                else {
                    cout<<"b"<<endl;
                }
            }
            total_profit += max_cycle_detected.second;
        }
        else {
            cout<<"No Trade"<<endl;
            continue;
        }
    }
    if (message[message.size()-1] == '$') {
                break;
    }

    previous_size = all_trades.size();

    }
    rcv.terminate();
    cout<<total_profit;

    }
}