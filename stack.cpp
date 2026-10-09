#include "stack.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

RideStackNode::RideStackNode(const RideRecord& r)
    : data(r), prev(nullptr), next(nullptr) {}

RideHistoryStack::RideHistoryStack() : topNode(nullptr) {}
RideHistoryStack::~RideHistoryStack() { clr(); }

void RideHistoryStack::psh(const RideRecord& data) {
    RideStackNode* newNode = new RideStackNode(data);
    if (topNode) {
        newNode->next = topNode;
        topNode->prev = newNode;
    }
    topNode = newNode;
}

RideRecord RideHistoryStack::pp() {
    if (emp()) return RideRecord();
    RideStackNode* temp = topNode;
    RideRecord val = temp->data;
    topNode = topNode->next;
    if (topNode) topNode->prev = nullptr;
    delete temp;
    return val;
}

bool RideHistoryStack::emp() const { return topNode == nullptr; }

void RideHistoryStack::clr() {
    while (!emp()) pp();
}


static bool ldRds(RideHistoryStack& st, const string& username = "", bool isDriver = false) {
    ifstream fin("rides.csv");
    if (!fin) return false;

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string p[8];
        for (int i = 0; i < 8 && getline(ss, p[i], ','); i++);

        if (!username.empty()) {
            bool match = isDriver ? (p[2] == username) : (p[1] == username);
            if (!match) continue;
        }

        RideRecord r;
        r.timestamp = p[0]; r.user = p[1]; r.driver = p[2];
        r.from = p[3]; r.to = p[4]; r.vehicle = p[7];
        try {
            r.distance = stod(p[5]);
            r.fare = stod(p[6]);
        } catch (...) {}
        st.psh(r);
    }
    return true;
}

//user driver er
void shwHist(const string& username, bool isDriver) {
    RideHistoryStack st;
    if (!ldRds(st, username, isDriver) || st.emp()) {
        cout << (isDriver ? "No completed rides yet.\n" : "No trips yet.\n");
        return;
    }

    cout << (isDriver ? "\n--- YOUR COMPLETED RIDES (Most Recent First via Stack [LIFO]) ---\n"
                      : "\n--- YOUR TRIP HISTORY (Most Recent First via Stack [LIFO]) ---\n");
    int rank = 1;
    while (!st.emp()) {
        RideRecord r = st.pp();
        cout << rank++ << ". " << r.timestamp << " | "
             << (isDriver ? ("User: " + r.user) : ("Driver: " + r.driver)) << " | "
             << r.from << " -> " << r.to << " | " << r.vehicle
             << " | Fare: " << r.fare << " taka\n";
    }
}

// admin er
void shwHist() {
    RideHistoryStack st;
    if (!ldRds(st) || st.emp()) {
        cout << "No rides found.\n";
        return;
    }

    cout << "\n--- ALL COMPLETED RIDES LOG (Most Recent First via Stack [LIFO]) ---\n";
    int rank = 1;
    while (!st.emp()) {
        RideRecord r = st.pp();
        cout << rank++ << ". " << r.timestamp << " | Passenger: " << r.user
             << " | Driver: " << r.driver << " | "
             << r.from << " -> " << r.to << " | " << r.distance << " km | "
             << r.fare << " BDT | Vehicle: " << r.vehicle << "\n";
    }
}
