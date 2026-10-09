#include "bst.hpp"
#include "driver.hpp"


UserBSTNode* UserAccountBST::insrthlpr(UserBSTNode* node, int id, const std::string& username) {
    if (!node) {
        return new UserBSTNode(id, username);
    }
    if (id < node->id) {
        node->left = insrthlpr(node->left, id, username);
    } else if (id > node->id) {
        node->right = insrthlpr(node->right, id, username);
    } else {
        node->username = username;
    }
    return node;
}

UserBSTNode* UserAccountBST::srchhlpr(UserBSTNode* node, int id) const {
    if (!node || node->id == id) return node;
    if (id < node->id) return srchhlpr(node->left, id);
    return srchhlpr(node->right, id);
}

UserBSTNode* UserAccountBST::fndminnd(UserBSTNode* node) const {
    while (node && node->left) node = node->left;
    return node;
}

UserBSTNode* UserAccountBST::delhlpr(UserBSTNode* node, int id, bool& deleted) {
    if (!node) {
        deleted = false;
        return nullptr;
    }
    if (id < node->id) {
        node->left = delhlpr(node->left, id, deleted);
    } else if (id > node->id) {
        node->right = delhlpr(node->right, id, deleted);
    } else {
        deleted = true;
        if (!node->left) {
            UserBSTNode* temp = node->right;
            delete node;
            return temp;
        } else if (!node->right) {
            UserBSTNode* temp = node->left;
            delete node;
            return temp;
        }
        UserBSTNode* succ = fndminnd(node->right);
        node->id = succ->id;
        node->username = succ->username;
        bool dummy = false;
        node->right = delhlpr(node->right, succ->id, dummy);
    }
    return node;
}

void UserAccountBST::dstryhlpr(UserBSTNode* node) {
    if (!node) return;
    dstryhlpr(node->left);
    dstryhlpr(node->right);
    delete node;
}

// Public Methods
UserAccountBST::UserAccountBST() : root(nullptr) {}

UserAccountBST::~UserAccountBST() {
    clr();
}

void UserAccountBST::insrt(int id, const std::string& username) {
    root = insrthlpr(root, id, username);
}

std::string* UserAccountBST::srch(int id) {
    UserBSTNode* node = srchhlpr(root, id);
    return node ? &(node->username) : nullptr;
}

bool UserAccountBST::rmv(int id) {
    bool deleted = false;
    root = delhlpr(root, id, deleted);
    return deleted;
}

void UserAccountBST::clr() {
    dstryhlpr(root);
    root = nullptr;
}

// DSA Use-Case: Recursive Binary Search Algorithm for Driver Lookup (O(log N))
int BnrSrchRcrsn(const DriverInfo* arr, int low, int high, int targetId) {
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    if (arr[mid].id == targetId) return mid;
    if (arr[mid].id > targetId) 
        return BnrSrchRcrsn(arr, low, mid - 1, targetId);
    return BnrSrchRcrsn(arr, mid + 1, high, targetId);
}
