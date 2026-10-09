#pragma once

#include <string>

class RideRecord {
public:
    std::string timestamp, user, driver, from, to, vehicle;
    double distance = 0.0, fare = 0.0;
};

//g Doubly Linked List stack er jonno
struct RideStackNode {
    RideRecord data;
    RideStackNode *prev, *next;
    RideStackNode(const RideRecord& r);
};

//main stacck
class RideHistoryStack {
private:
    RideStackNode* topNode;

public:
    RideHistoryStack();
    ~RideHistoryStack();

    void psh(const RideRecord& data);
    RideRecord pp();
    bool emp() const;
    void clr();
};

void shwHist(const std::string& username, bool isDriver); // Rider & Driver
void shwHist(); // Admin
