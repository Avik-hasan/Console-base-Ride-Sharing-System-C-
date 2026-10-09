#include "driver.hpp"
#include "sorting.hpp"
#include "bst.hpp"

DriverModule::DriverModule() {}

DriverModule::~DriverModule() {}

bool DriverModule::adddrv(string username, string password, string vehicleType, string location) {
    if (fnddrv(username) != -1) return false;
    
    DriverInfo d;
    d.id = 1;
    for (int i = 0; i < drivers.sz(); ++i) {
        if (drivers[i].id >= d.id) d.id = drivers[i].id + 1;
    }
    d.username = username;
    d.password = password;
    d.stVeh(vehicleType);
    d.location = location;
    d.available = true;
    d.totalEarnings = 0.0;
    d.tripCount = 0;
    d.avgRating = 0.0;
    d.ratingCount = 0;
    
    drivers.pshbk(d);
    return true;
}

bool DriverModule::rmvdrv(string username) {
    int idx = fnddrv(username);
    if (idx == -1) return false;

    drivers.ers(idx);
    return true;
}

int DriverModule::fnddrv(string username) {
    for (int i = 0; i < drivers.sz(); i++) {
        if (drivers[i].username == username) return i;
    }
    return -1;
}

DriverInfo* DriverModule::gtdr(string username) {
    int idx = fnddrv(username);
    return (idx != -1) ? &drivers[idx] : nullptr;
}


DriverInfo* DriverModule::gtdrID(int targetId) {
    int idx = ::BnrSrchRcrsn(drivers.dtptr(), 0, drivers.sz() - 1, targetId);
    return (idx != -1) ? &drivers[idx] : nullptr;
}

void DriverModule::shwall() {
    cout << "\n--- All Drivers ---\n";
    for (int i = 0; i < drivers.sz(); i++) {
        drivers[i].shwProf();
    }
}

void DriverModule::shwsts(string username) {
    DriverInfo* d = gtdr(username);
    if (!d) { cout << "Driver not found!\n"; return; }
    cout << "\n--- Stats for " << username << " ---\n"
         << "Vehicle: " << d->vehicleType << "\nLocation: " << d->location 
         << "\nStatus: " << (d->available ? "Online" : "Offline") 
         << "\nTotal Earnings: " << d->totalEarnings << " BDT\nTotal Trips: " << d->tripCount 
         << "\nAvg Rating: " << d->avgRating << " (" << d->ratingCount << " reviews)\n";
}

void DriverModule::gtbyveh(string vehicleType, Vector<int>& indices) {
    indices.clr();
    for (int i = 0; i < drivers.sz(); i++) {
        if (drivers[i].vehicleType == vehicleType && drivers[i].available) {
            indices.pshbk(i);
        }
    }
}

void DriverModule::ldrbrd(Vector<int>& indices, int maxCount) {
    indices.clr();
    int count = (drivers.sz() < maxCount) ? drivers.sz() : maxCount;
    if (count == 0) return;

    Vector<int> all;
    for (int i = 0; i < drivers.sz(); i++) all.pshbk(i);

    mrgesrt(all.dtptr(), 0, drivers.sz() - 1, drivers.dtptr());

    for (int i = 0; i < count; i++) {
        indices.pshbk(all[i]);
    }
}

void DriverModule::ld(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string f[10];
        int col = 0;
        while (col < 10 && getline(ss, f[col], ',')) col++;

        if (col >= 9) {
            DriverInfo d;
            int off = (isdigit(f[0][0]) && col == 10) ? 1 : 0;
            d.id = (off == 1) ? stoi(f[0]) : drivers.sz() + 1;
            d.username = f[off];
            d.password = f[off + 1];
            d.stVeh(f[off + 2]);
            d.location = f[off + 3];
            d.available = (f[off + 4] == "1");

            try {
                d.totalEarnings = stod(f[off + 5]);
                d.tripCount = stoi(f[off + 6]);
                d.avgRating = stod(f[off + 7]);
                d.ratingCount = stoi(f[off + 8]);
            } catch (...) {}
            drivers.pshbk(d);
        }
    }
}

void DriverModule::svfl(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return;

    for (int i = 0; i < drivers.sz(); i++) {
        file << drivers[i].id << "," << drivers[i].username << "," << drivers[i].password << ","
             << drivers[i].vehicleType << "," << drivers[i].location << ","
             << (drivers[i].available ? "1" : "0") << "," << drivers[i].totalEarnings << ","
             << drivers[i].tripCount << "," << drivers[i].avgRating << ","
             << drivers[i].ratingCount << "\n";
    }
}
