#include<iostream>
#include<vector>
#include<string>
#include<cstring>

using namespace std;

struct trade_line {
    string broker_name;
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
};

vector<order_executed> all_orders_completed;

vector <string> stock_names;

vector <trade_line> all_trades;

bool trade_line_match(int trade_line_1, int trade_line_2) {
    if (all_trades[trade_line_1].participating == false || all_trades[trade_line_2].participating == false) return false;
    if (all_trades[trade_line_1].action == all_trades[trade_line_2].action) return false;
    for (int i = 0 ; i < stock_names.size() ; i ++) {
        if (all_trades[trade_line_1].stock_quantity[i] != all_trades[trade_line_2].stock_quantity[i] ) return false;
    }
    return true;
}

void increasing_sort_all_matches (vector<int> &trade_lines_those_match) {
    int n = trade_lines_those_match.size();
    for (int i = 1 ; i < n ; i ++) {
        int key = trade_lines_those_match[i];
        int j = i - 1;
        while (j >= 0 && all_trades[trade_lines_those_match[j]].package_price > all_trades[key].package_price) {
            trade_lines_those_match[j + 1] = trade_lines_those_match[j]; 
            j --;
        }
        trade_lines_those_match[j+1] = key;
    }
}

void decreasing_sort_matches (vector<int> &trade_lines_those_match) {
    int n = trade_lines_those_match.size();
    for (int i = 1 ; i < n ; i ++) {
        int key = trade_lines_those_match[i];
        int j = i - 1;
        while (j >= 0 && all_trades[trade_lines_those_match[j]].package_price < all_trades[key].package_price) {
            trade_lines_those_match[j + 1] = trade_lines_those_match[j]; 
            j --;
        }
        trade_lines_those_match[j+1] = key;
    }
}

int main() {
    // STRING PARSING



    for (int i = 1 ; i < all_trades.size() ; i ++) {
        for (int j = 0 ; j < i ; j ++) {
            if (all_trades[j].start_time + all_trades[j].expiration_period < all_trades[i].start_time ||all_trades[j].total_quantity == 0) {
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
                break;
            }
        }
        if (lines_those_match.size() == 0) break;
        if (all_trades[i].action == "b") {
            increasing_sort_all_matches(lines_those_match);
            int l = 0;
            while (l < lines_those_match.size()) {
                if (all_trades[i].total_quantity <= all_trades[lines_those_match[l]].total_quantity) {
                    order_executed new_order_to_be_appended;
                    new_order_to_be_appended.buyer = i;
                    new_order_to_be_appended.seller = lines_those_match[l];
                    new_order_to_be_appended.price = all_trades[lines_those_match[l]].package_price;
                    new_order_to_be_appended.quantity_exchanged = all_trades[i].total_quantity;
                    all_orders_completed.push_back(new_order_to_be_appended);

                    all_trades[i].total_quantity = 0;
                    all_trades[lines_those_match[l]].total_quantity -= all_trades[i].total_quantity;
                    
                    break;
                }
                else {
                    order_executed new_order_to_be_appended;
                    new_order_to_be_appended.buyer = i;
                    new_order_to_be_appended.seller = lines_those_match[l];
                    new_order_to_be_appended.price = all_trades[lines_those_match[l]].package_price;
                    new_order_to_be_appended.quantity_exchanged = all_trades[lines_those_match[l]].total_quantity;
                    all_orders_completed.push_back(new_order_to_be_appended);

                    all_trades[i].total_quantity -= all_trades[lines_those_match[l]].total_quantity;
                    all_trades[lines_those_match[l]].total_quantity = 0;
                }
                l ++;
            }
        }
        else if (all_trades[i].action == "s") {
            decreasing_sort_matches(lines_those_match);
            int l = 0;
            while (l < lines_those_match.size()) {
                if (all_trades[i].total_quantity <= all_trades[lines_those_match[l]].total_quantity) {
                    order_executed new_order_to_be_appended;
                    new_order_to_be_appended.seller = i;
                    new_order_to_be_appended.buyer = lines_those_match[l];
                    new_order_to_be_appended.price = all_trades[lines_those_match[l]].package_price;
                    new_order_to_be_appended.quantity_exchanged = all_trades[i].total_quantity;
                    all_orders_completed.push_back(new_order_to_be_appended);

                    all_trades[i].total_quantity = 0;
                    all_trades[lines_those_match[l]].total_quantity -= all_trades[i].total_quantity;
                    break;
                }
                else {
                    order_executed new_order_to_be_appended;
                    new_order_to_be_appended.seller = i;
                    new_order_to_be_appended.buyer = lines_those_match[l];
                    new_order_to_be_appended.price = all_trades[lines_those_match[l]].package_price;
                    new_order_to_be_appended.quantity_exchanged = all_trades[lines_those_match[l]].total_quantity;
                    all_orders_completed.push_back(new_order_to_be_appended);

                    all_trades[i].total_quantity -= all_trades[lines_those_match[l]].total_quantity;
                    all_trades[lines_those_match[l]].total_quantity = 0;
                }
                l ++;
            }
        }
    }

    
}