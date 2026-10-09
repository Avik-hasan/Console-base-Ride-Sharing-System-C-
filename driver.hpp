#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "vector.hpp"
#include "user.hpp"
#include "vehicle.hpp"

using namespace std;


class DriverInfo : public User {
public:
    string vehicleType;  // Bike, CNG, UberX
    shared_ptr<Vehicle> vehicle; // Polymorphic Vehicle
    string location;     // current city name
    bool available;      // online/offline
    double totalEarnings;
    int tripCount;
    double avgRating;
    int ratingCount;

    DriverInfo() : User(), vehicleType(""), vehicle(nullptr), location(""), available(true), totalEarnings(0.0), tripCount(0), avgRating(5.0), ratingCount(0) {}

    void stVeh(const string& type) {
        vehicleType = type;
        vehicle = mkVeh(type);
    }

    void shwProf() const override {//user r driver duikhanei ase,override kora function
        cout << "ID: " << id << " | Name: " << username 
             << " | Vehicle: " << (vehicle ? vehicle->getType() : vehicleType) << " | City: " << location 
             << " | Rating: " << avgRating << " | Trips: " << tripCount 
             << " | Status: " << (available ? "Online" : "Offline") << "\n";
    }
};

class DriverModule {
public:
    Vector<DriverInfo> drivers;    

    DriverModule();
    ~DriverModule();
    
    bool adddrv(string username, string password, string vehicleType, string location);
    bool rmvdrv(string username);
    int fnddrv(string username);
    DriverInfo* gtdr(string username);
    

    DriverInfo* gtdrID(int targetId);
    
    void tglavlblty(string username) {
        DriverInfo* d = gtdr(username);
        if (d) d->available = !d->available;
    }
    
    void stlc(string username, string newLocation) {
        DriverInfo* d = gtdr(username);
        if (d) d->location = newLocation;
    }
    
    void addern(string username, double earning) {
        DriverInfo* d = gtdr(username);
        if (d) { d->tripCount++; d->totalEarnings += earning; }
    }
    
    void rte(string username, int rating) {
        DriverInfo* d = gtdr(username);
        if (d && rating >= 1 && rating <= 5) {
            double total = d->avgRating * d->ratingCount;
            d->ratingCount++;
            d->avgRating = (total + rating) / d->ratingCount;
        }
    }
    
    void shwall();
    void shwsts(string username);
    
    void gtbyveh(string vehicleType, Vector<int>& indices);
    void ldrbrd(Vector<int>& indices, int maxCount = 5);
    
    void ld(const string& filename);
    void svfl(const string& filename);
};
