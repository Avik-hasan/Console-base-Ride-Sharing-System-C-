#pragma once

#include <string>
#include <iostream>

using namespace std;
//abstract class banaisi 2 ta class er jonno user and driver 
class User {
public:
    int id;
    string username;
    string password;

    User() : id(0), username(""), password("") {}
    User(int id, string u, string p) : id(id), username(u), password(p) {}
    virtual ~User() {}
    virtual void shwProf() const = 0;
};
