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
    string brokerName;
    vector <stock_info> equation_parameters;
    vector<int> stock_quantity;
    string action;
    int package_price;
    bool participating = true;
    int total_quantity;
    int start_time;
    int expiration_period;
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

bool equation_parameter_matcher (vector<int> equation1, vector<int> equation2) {
    for (int i = 0 ; i < equation1.size() ; i ++ ) {
        if (equation1[i] != equation2[i]) {
            return false;
        }
    }
    return true;
}




int main () {
    vector<vector <int> > stock_entities;
    vector<vector<int> > corresponding_prices;

    vector<trade_line> all_user_trades;
    string user_broker_name = "22B1003_22B0933";
    vector<int> check_trade_lines;
    for (int i = 0 ; i < all_trades.size() ; i ++) {
        check_trade_lines.push_back(i);
        bool found_this_equation = false;
        int found_position = corresponding_prices.size();
        for (int m = 0 ; m < stock_entities.size() ; m ++) {
            if (equation_parameter_matcher(all_trades[i].stock_quantity, stock_entities[m])) {
                found_this_equation = true;
                found_position = m;
                for (int y = 0 ; y < corresponding_prices[m].size() ; y ++ ) {
                    if (corresponding_prices[m][y] < all_trades[i].package_price) {
                        continue;
                    }
                    else {
                        corresponding_prices[m].insert(corresponding_prices[m].begin() + y, all_trades[i].package_price);
                        break;
                    }
                }
                break;
            }
        }
        
        if (!found_this_equation) {
            stock_entities.insert(all_trades[i].stock_quantity);
            corresponding_prices.insert(all_trades[i].package_price);
        }

        if ( i == 0 ) {
            continue;
        }
        for (int j = 0 ; j < i ; j ++) {
            if ((all_trades[j].start_time + all_trades[j].expiration_period < all_trades[i].start_time && all_trades[j].expiration_period != -1) ||all_trades[j].total_quantity == 0) {
                all_trades[j].participating = false;
            }
        }

        if (max_cycle_detector(check_trade_lines).first.first.size() > 0 ) {
            int minimum_expiration_time = INT16_MAX;
            for (int j = 0 ; j < max_cycle_detector(check_trade_lines).first.first.size() ; j ++) {
                if (all_trades[max_cycle_detector(check_trade_lines).first.first[j]].expiration_period == -1) continue;
                if (all_trades[max_cycle_detector(check_trade_lines).first.first[j]].expiration_period < minimum_expiration_time) minimum_expiration_time = all_trades[max_cycle_detector(check_trade_lines).first.first[j]].expiration_period;
            }

            for (int j = 0 ; j < max_cycle_detector(check_trade_lines).first.first.size() ; j ++) {
                trade_line user_trade;
                user_trade.brokerName = user_broker_name;
                if (all_trades[max_cycle_detector(check_trade_lines).first.first[j]].action == "b") {
                    user_trade.action = "s";
                }
                else if (all_trades[max_cycle_detector(check_trade_lines).first.first[j]].action == "s") {
                    user_trade.action = "b";
                }
                user_trade.equation_parameters = all_trades[max_cycle_detector(check_trade_lines).first.first[j]].equation_parameters;
                user_trade.package_price = all_trades[max_cycle_detector(check_trade_lines).first.first[j]].package_price;
                user_trade.stock_quantity = all_trades[max_cycle_detector(check_trade_lines).first.first[j]].stock_quantity;
                user_trade.total_quantity = max_cycle_detector(check_trade_lines).first.second[j];
                user_trade.start_time = all_trades[max_cycle_detector(check_trade_lines).first.first[max_cycle_detector(check_trade_lines).first.first.size() - 1]].start_time;
                user_trade.expiration_period = minimum_expiration_time;
                all_user_trades.push_back(user_trade);
                if (all_trades[max_cycle_detector(check_trade_lines).first.first[j]].total_quantity == user_trade.total_quantity) {
                    all_trades[max_cycle_detector(check_trade_lines).first.first[j]].participating = false;
                }
                else {
                    all_trades[max_cycle_detector(check_trade_lines).first.first[j]].total_quantity -= user_trade.total_quantity
                }
            }
            continue;
        }

        if (all_trades[i].action == "b" && all_trades[i].package_price > corresponding_prices[found_position][corresponding_prices[found_position].size()/2]) {
            trade_line user_trade;
            user_trade.brokerName = user_broker_name;
            user_trade.action = "s";
            user_trade.equation_parameters = all_trades[i].equation_parameters;
            user_trade.package_price = all_trades[i].package_price;
            user_trade.start_time = all_trades[i].start_time;
            user_trade.expiration_period = all_trades[i].expiration_period;
            user_trade.stock_quantity = all_trades[i].stock_quantity;
            user_trade.total_quantity = all_trades[i].total_quantity;
            all_user_trades.push_back(user_trade);
            all_trades[i].participating = false;
        }

        else if (all_trades[i].action == "s" && all_trades[i].package_price < corresponding_prices[found_position][corresponding_prices[found_position].size()/2]) {
            trade_line user_trade;
            user_trade.brokerName = user_broker_name;
            user_trade.action = "b";
            user_trade.equation_parameters = all_trades[i].equation_parameters;
            user_trade.package_price = all_trades[i].package_price;
            user_trade.start_time = all_trades[i].start_time;
            user_trade.expiration_period = all_trades[i].expiration_period;
            user_trade.stock_quantity = all_trades[i].stock_quantity;
            user_trade.total_quantity = all_trades[i].total_quantity;
            all_user_trades.push_back(user_trade);
            all_trades[i].participating = false;
        }
    }
    for (int l = 0 ; l < all_user_trades.size() ; l ++ ) {
        // 0 KarGoCorp SELL AMZN $100 #30 10
        cout<<all_user_trades[l].start_time<<" "<<all_user_trades[l].brokerName<<" ";
        if (all_user_trades[l].action == "s") {
            cout<<"SELL ";
        }
        else {
            cout<<"BUY ";
        }
        if (all_user_trades[l].equation_parameters.size() == 1) {
            cout<<all_user_trades[l].equation_parameters[0].companyName<<" ";
        }
        else {
            for (int q = 0 ; q < all_user_trades[l].equation_parameters.size() ; q ++ ){
                cout<<all_user_trades[l].equation_parameters[q]<<" "<<all_user_trades[l].equation_parameters[q]<<" ";
            }
        }
        cout<<"$"<<all_user_trades[l].package_price<<" #"<<all_user_trades[l].total_quantity<<" "<<all_user_trades[l].expiration_period<<endl;
    }
}