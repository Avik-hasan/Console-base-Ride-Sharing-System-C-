#include "queue.hpp"

WalletRequestQueue::WalletRequestQueue() : frontNode(nullptr), rearNode(nullptr), count(0) {}

WalletRequestQueue::~WalletRequestQueue() {
    clr();
}

void WalletRequestQueue::enq(const WalletRequest& data) {
    WalletQueueNode* newNode = new WalletQueueNode(data);
    if (emp()) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        newNode->prev = rearNode;
        rearNode = newNode;
    }
    count++;
}

WalletRequest WalletRequestQueue::deq() {
    if (!frontNode) return WalletRequest();
    WalletQueueNode* temp = frontNode;
    WalletRequest val = temp->data;
    frontNode = frontNode->next;
    if (frontNode) {
        frontNode->prev = nullptr;
    } else {
        rearNode = nullptr;
    }
    delete temp;
    count--;
    return val;
}

bool WalletRequestQueue::emp() const {
    return frontNode == nullptr;
}

int WalletRequestQueue::gtsz() const {
    return count;
}

WalletQueueNode* WalletRequestQueue::frntnd() const {
    return frontNode;
}

void WalletRequestQueue::clr() {
    while (!emp()) {
        deq();
    }
}
BFSQueue::BFSQueue() : frontNode(nullptr), rearNode(nullptr) {}

BFSQueue::~BFSQueue() {
    clr();
}

void BFSQueue::enq(int data) {
    BFSQueueNode* newNode = new BFSQueueNode(data);
    if (emp()) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        newNode->prev = rearNode;
        rearNode = newNode;
    }
}

int BFSQueue::deq() {
    if (emp()) return -1;
    BFSQueueNode* temp = frontNode;
    int val = temp->data;
    frontNode = frontNode->next;
    if (frontNode) {
        frontNode->prev = nullptr;
    } else {
        rearNode = nullptr;
    }
    delete temp;
    return val;
}

bool BFSQueue::emp() const {
    return frontNode == nullptr;
}

void BFSQueue::clr() {
    while (!emp()) {
        deq();
    }
}
