#pragma once

#include <string>

// User ID -> Username Lookup Node
class UserBSTNode {
public:
    int id;
    std::string username;
    UserBSTNode* left;
    UserBSTNode* right;

    UserBSTNode(int id, const std::string& u)
        : id(id), username(u), left(nullptr), right(nullptr) {}
};

// Binary Search Tree (BST) for Fast User ID Lookup
class UserAccountBST {
private:
    UserBSTNode* root;

    UserBSTNode* insrthlpr(UserBSTNode* node, int id, const std::string& username);
    UserBSTNode* srchhlpr(UserBSTNode* node, int id) const;
    UserBSTNode* fndminnd(UserBSTNode* node) const;
    UserBSTNode* delhlpr(UserBSTNode* node, int id, bool& deleted);
    void dstryhlpr(UserBSTNode* node);

public:
    UserAccountBST();
    ~UserAccountBST();

    void insrt(int id, const std::string& username);
    std::string* srch(int id);
    bool rmv(int id);
    void clr();
};

// driver id diye binary search diye khujbo driver module e
class DriverInfo;
int BnrSrchRcrsn(const DriverInfo* arr, int low, int high, int targetId);
