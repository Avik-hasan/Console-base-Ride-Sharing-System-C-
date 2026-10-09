#include "payment.hpp"
#include "rider.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

PaymentSystem::PaymentSystem() {
  companyRevenue = 0.0;
  commissionRate = 0.20;
  ldPrc("pricing_config.csv");
}

void PaymentSystem::addrq(string username, double amount) {
  WalletRequest req(username, amount, "Pending");
  pendingQueue.enq(req);
  allRequests.insrtTl(req);
}

static WalletRequest procRq(WalletRequestQueue& queue, SinglyLinkedList<WalletRequest>& allRequests, const string& newStatus) {
  WalletRequest req = queue.deq();
  for (ListNode<WalletRequest> *curr = allRequests.head(); curr; curr = curr->next) {
    if (curr->data.username == req.username && curr->data.amount == req.amount && curr->data.status == "Pending") {
      curr->data.status = newStatus;
      break;
    }
  }
  return req;
}

bool PaymentSystem::apprv(RiderModule *rm) {
  if (pendingQueue.emp()) {
    cout << "\nNo pending wallet requests in queue.\n";
    return false;
  }

  WalletRequest req = procRq(pendingQueue, allRequests, "Approved");
  if (rm) {
    rm->addMny(req.username, req.amount);
    rm->sv("users.csv");
  }
  cout << "\n[FIFO Queue] Approved " << req.amount << " taka for user: " << req.username << "\n";
  return true;
}

bool PaymentSystem::rjct() {
  if (pendingQueue.emp()) {
    cout << "\nNo pending wallet requests in queue.\n";
    return false;
  }

  WalletRequest req = procRq(pendingQueue, allRequests, "Rejected");
  cout << "\n[FIFO Queue] Rejected " << req.amount << " taka for user: " << req.username << "\n";
  return true;
}

void PaymentSystem::shwPnd() {
  if (pendingQueue.emp()) {
    cout << "No pending wallet requests.\n";
    return;
  }

  WalletQueueNode *curr = pendingQueue.frntnd();
  while (curr) {
    cout << "Username: " << curr->data.username << ", Amount: " << curr->data.amount << "\n";
    curr = curr->next;
  }
}

int PaymentSystem::pndng() { return pendingQueue.gtsz(); }

void PaymentSystem::updPrc(string vehicleType, double newBase, double newPerKm) {
  double *b = nullptr, *km = nullptr;
  if (vehicleType == "Bike" || vehicleType == "1") { b = &bikeBase; km = &bikePerKm; }
  else if (vehicleType == "CNG" || vehicleType == "2") { b = &cngBase; km = &cngPerKm; }
  else if (vehicleType == "UberX" || vehicleType == "3") { b = &uberxBase; km = &uberxPerKm; }
  else {
    cout << "Invalid vehicle type specified.\n";
    return;
  }
  *b = newBase;
  *km = newPerKm;
  cout << vehicleType << " pricing updated successfully!\n";
}

void PaymentSystem::shwPrc() {
  cout << "\n--- CURRENT PRICING & SURGE CONFIGURATION ---\n";
  cout << "1. Bike   : Base = " << bikeBase << " taka, Per-km = " << bikePerKm << " taka\n";
  cout << "2. CNG    : Base = " << cngBase << " taka, Per-km = " << cngPerKm << " taka\n";
  cout << "3. UberX  : Base = " << uberxBase << " taka, Per-km = " << uberxPerKm << " taka\n";
  cout << "--- SURGE PARAMETERS ---\n";
  cout << "Demand Surge Weight   : " << demandWeight << "\n";
  cout << "Max Surge Cap         : " << maxSurgeCap << "\n";
}

double PaymentSystem::calcFare(double distance, shared_ptr<Vehicle> vehicle,
                                    double trafficMul, double weatherMul,
                                    int availableDrivers, int totalUsers) {
  double baseFare = bikeBase, perKm = bikePerKm;
  if (vehicle) {
    if (vehicle->getType() == "CNG") { baseFare = cngBase; perKm = cngPerKm; }
    else if (vehicle->getType() == "UberX") { baseFare = uberxBase; perKm = uberxPerKm; }
  }

  double rawFare = baseFare + (distance * perKm);
  double trafficSurge = trafficMul;
  double weatherSurge = weatherMul;
  double demandSurge = (availableDrivers > 0 && totalUsers > availableDrivers)
                       ? ((double)totalUsers / availableDrivers - 1.0) * demandWeight : 0.0;

  double totalSurge = min(maxSurgeCap, trafficSurge + weatherSurge + demandSurge);
  return rawFare * (1.0 + totalSurge);
}

void PaymentSystem::addRev(double fare) {
  companyRevenue += fare * commissionRate;
}

void PaymentSystem::ldRev() {
  ifstream file("rides.csv");
  if (!file.is_open()) return;
  string line;
  while (getline(file, line)) {
    if (line.empty()) continue;
    stringstream ss(line);
    string f[8];
    int col = 0;
    while (col < 8 && getline(ss, f[col], ',')) col++;
    if (col > 6) {
      try {
        double fare = stod(f[6]);
        companyRevenue += fare * commissionRate;
      } catch (...) {}
    }
  }
}

double PaymentSystem::drvShr(double fare) {
  return fare * (1.0 - commissionRate);
}

void PaymentSystem::ldRq(const string &filename) {
  ifstream file(filename);
  if (!file.is_open()) return;

  string line;
  while (getline(file, line)) {
    if (line.empty()) continue;

    stringstream ss(line);
    string username, amountStr, status;
    if (getline(ss, username, ',') && getline(ss, amountStr, ',') && getline(ss, status, ',')) {
      try {
        double amount = stod(amountStr);
        WalletRequest req(username, amount, status);
        allRequests.insrtTl(req);
        if (status == "Pending") pendingQueue.enq(req);
      } catch (...) {}
    }
  }
}

void PaymentSystem::svRq(const string &filename) {
  ofstream file(filename);
  if (!file.is_open()) return;

  for (ListNode<WalletRequest> *curr = allRequests.head(); curr; curr = curr->next) {
    file << curr->data.username << "," << curr->data.amount << "," << curr->data.status << "\n";
  }
}

void PaymentSystem::ldPrc(const string &filename) {
  ifstream file(filename);
  if (!file.is_open()) return;

  string line;
  while (getline(file, line)) {
    if (line.empty()) continue;
    stringstream ss(line);
    string key, valStr;
    if (getline(ss, key, ',') && getline(ss, valStr, ',')) {
      try {
        double val = stod(valStr);
        if (key == "bike_base") bikeBase = val;
        else if (key == "bike_per_km") bikePerKm = val;
        else if (key == "cng_base") cngBase = val;
        else if (key == "cng_per_km") cngPerKm = val;
        else if (key == "uberx_base") uberxBase = val;
        else if (key == "uberx_per_km") uberxPerKm = val;
        else if (key == "traffic_factor") trafficFactor = val;
        else if (key == "weather_factor") weatherFactor = val;
        else if (key == "demand_weight") demandWeight = val;
        else if (key == "max_surge_cap") maxSurgeCap = val;
      } catch (...) {}
    }
  }
}

void PaymentSystem::svPrc(const string &filename) {
  ofstream file(filename);
  if (!file.is_open()) return;

  file << "key,value\n";
  file << "bike_base," << bikeBase << "\n";
  file << "bike_per_km," << bikePerKm << "\n";
  file << "cng_base," << cngBase << "\n";
  file << "cng_per_km," << cngPerKm << "\n";
  file << "uberx_base," << uberxBase << "\n";
  file << "uberx_per_km," << uberxPerKm << "\n";
  file << "traffic_factor," << trafficFactor << "\n";
  file << "weather_factor," << weatherFactor << "\n";
  file << "demand_weight," << demandWeight << "\n";
  file << "max_surge_cap," << maxSurgeCap << "\n";
}
