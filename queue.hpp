#pragma once

#include <string>

class WalletRequest {
public:
    std::string username;
    double amount;
    std::string status;

    WalletRequest(std::string u = "", double a = 0.0, std::string s = "Pending")
        : username(u), amount(a), status(s) {}
};

// Doubly Linked List banaisi walletrequest erjonno
struct WalletQueueNode {
    WalletRequest data;
    WalletQueueNode* prev;
    WalletQueueNode* next;
    WalletQueueNode(const WalletRequest& val) : data(val), prev(nullptr), next(nullptr) {}
};

// Wallet Request er Queue 
class WalletRequestQueue {
private:
    WalletQueueNode* frontNode;
    WalletQueueNode* rearNode;
    int count;

public:
    WalletRequestQueue();
    ~WalletRequestQueue();

    void enq(const WalletRequest& data);
    WalletRequest deq();
    bool emp() const;
    int gtsz() const;
    WalletQueueNode* frntnd() const;
    void clr();
};

// Doubly Linked List banaisi BFSQueue er jonno
struct BFSQueueNode {
    int data;
    BFSQueueNode* prev;
    BFSQueueNode* next;
    BFSQueueNode(int val) : data(val), prev(nullptr), next(nullptr) {}
};

// BFS er jonno Queue
class BFSQueue {
private:
    BFSQueueNode* frontNode;
    BFSQueueNode* rearNode;

public:
    BFSQueue();
    ~BFSQueue();

    void enq(int data);
    int deq();
    bool emp() const;
    void clr();
};
