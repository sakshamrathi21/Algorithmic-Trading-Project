#include<iostream>
#include<vector>
#include<string>

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




int main () {
    string message; 
    getline(cin, message);

    ll message_len = message.size();
    
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
        all_trades[j].stock_quantity = vector<int>(all_company_names.size());
        for(int g = 0; g < (all_trades[j]).equation_parameters.size(); g++){
            all_trades[j].stock_quantity[(all_trades[j]).equation_parameters[g].companyIndex] = (all_trades[j]).equation_parameters[g].quantity;
        }
    }

    stock_names = all_company_names;
    cout<<"No Trade"<<endl;
    int total_profit = 0;
    vector<int> check_trade_lines;
    check_trade_lines.push_back(0);
    for (int i = 1 ; i < all_trades.size() ; i ++) {
        check_trade_lines.push_back(i);
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
                    cout<<"s#"<<endl;
                }
                else {
                    cout<<"b#"<<endl;
                }
            }
            total_profit += max_cycle_detected.second;
        }
        else {
            cout<<"No Trade"<<endl;
            continue;
        }
    }
    cout<<total_profit;
}