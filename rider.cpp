#include "rider.hpp"
#include <fstream>
#include <sstream>

int RiderModule::fndrdr(string username) {
    for (int i = 0; i < riders.sz(); ++i) {
        if (riders[i].username == username) return i;
    }
    return -1;
}

RiderInfo* RiderModule::gtrdr(string username) {
    int idx = fndrdr(username);
    return (idx != -1) ? &riders[idx] : nullptr;
}

bool RiderModule::addrdr(string username, string password) {
    if (fndrdr(username) != -1) return false;

    RiderInfo r;
    r.id = (riders.sz() == 0) ? 1 : riders[riders.sz() - 1].id + 1;
    r.username = username;
    r.password = password;
    r.wallet = 1000.0;
    riders.pshbk(r);
    return true;
}

bool RiderModule::rmvrdr(string username) {
    int idx = fndrdr(username);
    if (idx == -1) return false;

    riders.ers(idx);
    return true;
}

bool RiderModule::ddcFare(string username, double fare) {
    RiderInfo* r = gtrdr(username);
    if (r && r->wallet >= fare) {
        r->wallet -= fare;
        return true;
    }
    return false;
}

void RiderModule::shwAll() {
    for (int i = 0; i < riders.sz(); ++i) {
        riders[i].shwProf();
    }
}

void RiderModule::ld(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string f[5];
        int col = 0;
        while (col < 5 && getline(ss, f[col], ',')) col++;

        if (col >= 4) {
            RiderInfo r;
            r.id = riders.sz() + 1;
            r.username = f[1];
            r.password = f[2];
            r.wallet = 0.0;

            try {
                r.id = stoi(f[0]);
                r.wallet = stod(f[3]);
            } catch (...) {}

            // Load favorites separated by semicolon
            if (col >= 5 && !f[4].empty()) {
                stringstream favSS(f[4]);
                string favItem;
                while (getline(favSS, favItem, ';')) {
                    if (!favItem.empty()) r.favorites.insrtTl(favItem);
                }
            }

            riders.pshbk(r);
        }
    }
}

void RiderModule::sv(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return;

    for (int i = 0; i < riders.sz(); ++i) {
        file << riders[i].id << "," << riders[i].username << "," << riders[i].password << ","
             << riders[i].wallet << ",";

        // Semisolon diye favorite location save korsi
        ListNode<string>* curr = riders[i].favorites.head();
        while (curr) {
            file << curr->data;
            if (curr->next) file << ";";
            curr = curr->next;
        }
        file << "\n";
    }
}
