#pragma once

#include <string>
#include <iostream>
#include "linkedlist.hpp"
#include "vector.hpp"
#include "user.hpp"

using namespace std;

// Derived clas
class RiderInfo : public User {
public:
    double wallet;
    SinglyLinkedList<string> favorites;

    RiderInfo() : User(), wallet(0.0) {}

    void shwProf() const override {
        cout << "ID: " << id << " | Name: " << username 
             << " | Balance: " << wallet << " taka\n";
    }
};

class RiderModule {
public:
    Vector<RiderInfo> riders;
    
    bool addrdr(string username, string password);
    bool rmvrdr(string username);
    int fndrdr(string username);
    RiderInfo* gtrdr(string username);
    
    bool ddcFare(string username, double fare);
    
    void addMny(string username, double amount) {
        RiderInfo* r = gtrdr(username);
        if (r) r->wallet += amount;
    }
    
    double bal(string username) {
        RiderInfo* r = gtrdr(username);
        return r ? r->wallet : 0.0;
    }
    
    void shwAll();
    
    void ld(const string& filename);
    void sv(const string& filename);
};
