#pragma once

#include <string>
#include <memory>
#include "queue.hpp"
#include "linkedlist.hpp"
#include "vehicle.hpp"

class RiderModule;

class PaymentSystem {
private:
    WalletRequestQueue pendingQueue;             // admin er queue request
    SinglyLinkedList<WalletRequest> allRequests; // all requests
    double commissionRate;                 // 20%

public:
    double companyRevenue;

    // vara nirnoy
    double bikeBase, bikePerKm, cngBase, cngPerKm, uberxBase, uberxPerKm;
    double trafficFactor, weatherFactor, demandWeight, maxSurgeCap;

    PaymentSystem();

    // Wallet requests manage kora
    void addrq(std::string username, double amount);
    bool apprv(RiderModule* rm = nullptr);
    bool rjct();
    void shwPnd();
    int pndng();

    void updPrc(std::string vehicleType, double newBase, double newPerKm);
    void shwPrc();

    double calcFare(double distance, std::shared_ptr<Vehicle> vehicle, double trafficMul,
                         double weatherMul, int availableDrivers, int totalUsers);

    void addRev(double fare);
    double drvShr(double fare);

    void ldPrc(const std::string& filename);
    void svPrc(const std::string& filename);

    void ldRq(const std::string& filename);
    void svRq(const std::string& filename);

    void ldRev();
};
