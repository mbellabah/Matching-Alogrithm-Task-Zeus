#include <iostream>
#include <map>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <math.h>
#include <list>
#include <time.h>
#include <stdio.h>
#include <vector>

using namespace std;


// MARK: Global Variables
int marketSize = 1000;


// MARK: Classes
class Buyer{
public:
    int address;
    double demand;
    Buyer(int x, double y);
};

Buyer::Buyer(int x, double y){
    this->address = x;
    this->demand = y;
}


class Seller{
public:
    int address;
    double supply;
    Seller(int x, double y);
};

Seller::Seller(int x, double y){
    this->address = x;
    this->supply = y;
}


// MARK: Dictionaries
map<int, Buyer*> buyer_dict;
map<int, Seller*> seller_dict;
map<string, double> matches_dict;


// MARK: Methods

void populateEntities(){
    for (int i = 1; i < marketSize; i = i + 1){
        buyer_dict[i] = new Buyer(i, rand() % 100 + 1);
        seller_dict[i] = new Seller(i, rand() % 100 + 1);
    }
}

void makeMatches(){
    matches_dict.clear();

    for (map<int, Buyer*>::iterator buyer_i = buyer_dict.begin(); buyer_i != buyer_dict.end(); ++buyer_i++){
        for (map<int , Seller*>::iterator seller_i = seller_dict.begin(); seller_i != seller_dict.end(); ++seller_i){
            int b_name = buyer_i->first;
            int s_name = seller_i->first;

            string match_name;
            {
                stringstream s1;
                stringstream s2;
                s1 << b_name;
                s2 << s_name;
                match_name = s1.str() + "," + s2.str();
            }
            double quantity_dev = fabs(buyer_i->second->demand - seller_i->second->supply);
            matches_dict[match_name] = quantity_dev;
        }
    }
}


int findBest(int buyer_addrs){
    list<double> quantityList;
    list<double>::iterator quantityListIterator;

    double smallest_q;

    if (matches_dict.size() > 0){
        for (map<string, double>::iterator matches_i = matches_dict.begin(); matches_i != matches_dict.end(); ++matches_i){

            string full_string = matches_i->first;
            size_t comma_indx = full_string.find(",");
            string buyer_tag = full_string.substr(0, comma_indx);
            int converted = strtol(buyer_tag.c_str(), NULL, 10);

            if (buyer_addrs == converted){
                quantityList.push_back(matches_i->second);
            }
        }
    }

    double temp = 1000.0;
    for (list<double>::iterator qIterator = quantityList.begin(); qIterator != quantityList.end(); qIterator++){
        if (*qIterator < temp){
            temp = *qIterator;
        }
    }
    smallest_q = temp;

    int match_seller;
    for (map<string, double>::iterator matches_i = matches_dict.begin(); matches_i != matches_dict.end(); ++matches_i){

        string full_string = matches_i->first;
        size_t comma_indx = full_string.find(",");

        int buyer_tag = strtol(full_string.substr(0,comma_indx).c_str(), NULL, 10);
        int seller_tag = strtol(full_string.substr(comma_indx+1).c_str(), NULL, 10);

        if ((buyer_addrs == buyer_tag) && (matches_i->second == smallest_q)){
                match_seller = seller_tag;
                break;
            }
    }

    return match_seller;
}


// MARK: Troubleshooting Methods
void displayMatches(){
    map<string, double>::iterator matches_i = matches_dict.begin();
    for (; matches_i != matches_dict.end(); ++matches_i){
        cout << matches_i->first << " => " << matches_i->second << endl;
        string small = matches_i->first.substr(0,1);
        int l = strtol(small.c_str(), NULL, 10);
        cout << l << endl;

    }
}
int main(){
    clock_t start = clock();

    srand(time(NULL));
    populateEntities();
    makeMatches();

    for (map<int, Buyer*>::iterator buyer_i = buyer_dict.begin(); buyer_i != buyer_dict.end(); ++buyer_i++){
        cout << "B: " << buyer_i->first << " => " << "S: " << findBest(buyer_i->first) << endl;
    }

    clock_t stop = clock();
    double elapsed = (double)(stop-start) * 1000.0 / CLOCKS_PER_SEC;
    printf("Time elapsed in ms: %f", elapsed);

    return 0;
}
