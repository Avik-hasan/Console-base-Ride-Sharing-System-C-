#pragma once

#include <string>
#include <memory>

using namespace std;

// Abstract base class banaisi 3type er vehicle e jeno redundant code/if-else na likhte hoy
class Vehicle {
public:
    virtual ~Vehicle() = default;

    virtual string getType() const = 0;
    virtual double getSpeed() const = 0;  
    virtual double getBaseFare() const = 0;   
    virtual double getPerKmRate() const = 0;  

    virtual double calcBaseFare(double distance) const {
        return getBaseFare() + (distance * getPerKmRate());
    }
};


class Bike : public Vehicle {
public:
    string getType() const override { return "Bike"; }
    double getSpeed() const override { return 25.0; }
    double getBaseFare() const override { return 60.0; }
    double getPerKmRate() const override { return 12.0; }
};


class CNG : public Vehicle {
public:
    string getType() const override { return "CNG"; }
    double getSpeed() const override { return 18.0; }
    double getBaseFare() const override { return 100.0; }
    double getPerKmRate() const override { return 25.0; }
};


class UberX : public Vehicle {
public:
    string getType() const override { return "UberX"; }
    double getSpeed() const override { return 16.0; }
    double getBaseFare() const override { return 150.0; }
    double getPerKmRate() const override { return 28.0; }
};


inline shared_ptr<Vehicle> mkVeh(const string& type) {
    if (type == "CNG") return make_shared<CNG>();
    if (type == "UberX") return make_shared<UberX>();
    return make_shared<Bike>();
}

inline shared_ptr<Vehicle> mkVeh(int choice) {
    if (choice == 2) return make_shared<CNG>();
    if (choice == 3) return make_shared<UberX>();
    return make_shared<Bike>();
}
