#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>
#include "admin.hpp"
#include "auth.hpp"
#include "driver.hpp"
#include "graph.hpp"
#include "heap.hpp"
#include "linkedlist.hpp"
#include "payment.hpp"
#include "queue.hpp"
#include "rider.hpp"
#include "sorting.hpp"
#include "stack.hpp"
#include "vector.hpp"
#include "vehicle.hpp"
using namespace std;
// Global object banaisi,jate call kora easy hoy
RiderModule rm;
DriverModule dm;
AuthSystem auth(&rm, &dm);
CityGraph cg;
PaymentSystem pm;
AdminModule adm;


       string now()
        {
  time_t t = time(NULL);
  string s = ctime(&t);
  s.pop_back();
        return s;
          }

int hr()
 {
  time_t t = time(NULL);
  return localtime(&t)->tm_hour;
        }

//overloaded 3ta function,to take iint,string,double input
string inp(const string& prompt)
 { cout << prompt;
  string s;
  while (getline(cin, s)) {
    if (!s.empty()) return s;
    cout << prompt;
  }
  return "";
      }

int inp(const string& prompt, int minVal, int maxVal)
 {  while (true) {
    string s = inp(prompt);
    try {      int val = stoi(s);
      if (val >= minVal && val <= maxVal) return val;} catch (...) {}
    cout << "Invalid choice! Enter (" << minVal << "-" << maxVal << "): ";}}


double inp(const string& prompt, double minVal)
 {  while (true) {
    string s = inp(prompt);
    try {      double val = stod(s);
      if (val >= minVal) return val; } catch (...) {}
    cout << "Invalid input! Enter positive number: ";}}

void paus() {  cout << "\nPress Enter to continue..." << flush;cin.get();}

void ldProf(const string& fn, Profile *arr, int &cnt) {  cnt = 0;  ifstream fin(fn); if (!fin) return; string line;
  while (getline(fin, line) && cnt < 24) {    if (line.empty()) continue;
    stringstream ss(line);string hStr, cond, mStr;  getline(ss, hStr, ',');getline(ss, cond, ','); getline(ss, mStr, ',');
 try {      int h = stoi(hStr);
      if (h >= 0 && h < 24) {
        arr[h].hour = h;
        arr[h].cond = cond;
        arr[h].mul = stod(mStr);
        if (h >= cnt) cnt = h + 1;      }    } catch (...) {}
  }
      }

double gtMul(Profile *arr, int cnt, int h) {if (h >= 0 && h < cnt && arr[h].hour == h) return arr[h].mul;return 0.0;}

string gtCond(Profile *arr, int cnt, int h, const string& def) {
  if (h >= 0 && h < cnt && arr[h].hour == h) return arr[h].cond;
  return def;}

void addRide(const RideRecord& r) {  ofstream fout("rides.csv", ios::app);
  fout << r.timestamp << "," << r.user << "," << r.driver << "," << r.from
       << "," << r.to << "," << r.distance << "," << r.fare << "," << r.vehicle << "\n";}
//minheap diye match driver
int mtchdrv(string pCity, string vType) { int pIdx = cg.fndcty(pCity);
  if (pIdx < 0) return -1;

  Vector<int> dArr;
  dm.gtbyveh(vType, dArr);
  if (dArr.emp()) return -1;
  DriverMatchingMinHeap minHeap;
  for (int d = 0; d < dArr.sz(); d++) {
    DriverInfo &dr = dm.drivers[dArr[d]];
int dCityIdx = cg.fndcty(dr.location);
    double dist = 999.0;
    if (dCityIdx >= 0) {
      Vector<int> path; double d2 = 0.0;
      if (cg.bfs(dCityIdx, pIdx, path, d2)) dist = d2;    }
    minHeap.psh(HeapItem(dArr[d], dist, dr.avgRating));
  } return minHeap.emp() ? -1 : minHeap.extMin().driverIndex;}
string selLoc(string username) {
  RiderInfo *r = rm.gtrdr(username);
  if (r && !r->favorites.emp()) {
    cout << "\n--- YOUR FAVORITE LOCATIONS (Singly LinkedList) ---\n";
    for (int i = 0; i < r->favorites.sz(); ++i) {
      cout << "  [" << i + 1 << "] " << r->favorites.get(i) << "\n"; }
    cout << "  -> Choose favorite (1-" << r->favorites.sz() << ") or 0 to select from full list: ";
    string fLine;    if (getline(cin, fLine) && !fLine.empty()) {
      try {        int fChoice = stoi(fLine);
        if (fChoice >= 1 && fChoice <= r->favorites.sz()) {
          return r->favorites.get(fChoice - 1);
        }
      } catch (...) {}}}

  cg.shwcty();
  while (true) {
    string input = inp("Enter city name (or number 1-" + to_string(cg.cities.sz()) + "): ");

    try {
      int num = stoi(input);
      if (num >= 1 && num <= cg.cities.sz()) {
        return cg.cities[num - 1];
      }
    } catch (...) {}

    int idx = cg.fndcty(input);
    if (idx >= 0) return cg.cities[idx];

    cout << "Location '" << input << "' not found. Please try again!\n";
  }
}

// book ride er function
void bokRde(string u) {
  cout << "\n--- SELECT PICKUP LOCATION ---\n";
  string from = selLoc(u);

  cout << "\n--- SELECT DESTINATION LOCATION ---\n";
  string to = selLoc(u);

  int fIdx = cg.fndcty(from);
  int tIdx = cg.fndcty(to);
  if (fIdx == tIdx) {
    cout << "Pickup and destination cannot be the same. Ride canceled.\n";
    return;
  }
  from = cg.cities[fIdx];
  to = cg.cities[tIdx];

  cout << "\nVehicle Types:\n";
  cout << "1. Bike\n";
  cout << "2. CNG\n";
  cout << "3. UberX\n";
  int vCh = inp("Choose vehicle (1-3): ", 1, 3);
  shared_ptr<Vehicle> selVehicle = mkVeh(vCh);
  string vType = selVehicle->getType();
  double speed = selVehicle->getSpeed();

  // DFS  diye node connected kina and alternative route khujtesi
  Vector<int> dfsRoute;
  cg.dfs(fIdx, tIdx, dfsRoute);
  if (dfsRoute.emp()) {
    cout << "No road connection found between " << from << " and " << to << ". Ride canceled.\n";
    return;
  }

  //  BFS diye sobchye optimized road khuja
  Vector<int> path; double dist = 0.0;
  cg.bfs(fIdx, tIdx, path, dist);

  cout << "\n--- ROUTE ANALYSIS ---\n";
  cout << "From: " << from << "\n";
  cout << "To  : " << to << "\n";
  cout << "Distance: " << dist << " km\n";

  cout << "Optimal Path (BFS Traversal via Queue): ";
  for (int i = 0; i < path.sz(); i++) {
    cout << cg.cities[path[i]];
    if (i < path.sz() - 1) cout << " -> "; else cout << "\n";
  }

  if (dfsRoute.sz() != path.sz()) {
    cout << "Alternative Route (DFS Traversal via Recursion): ";
    for (int i = 0; i < dfsRoute.sz(); i++) {
      cout << cg.cities[dfsRoute[i]];
      if (i < dfsRoute.sz() - 1) cout << " -> "; else cout << "\n";
    }
  }

  int h = hr();
  double tMul = gtMul(adm.trData, adm.trCnt, h);
  double wMul = gtMul(adm.wtData, adm.wtCnt, h);
  Vector<int> tmpDrivers;
  dm.gtbyveh(vType, tmpDrivers);
  int aDrivers = tmpDrivers.sz();

  double fare = pm.calcFare(dist, selVehicle, tMul, wMul, aDrivers, rm.riders.sz());
  double estTime = (dist / speed) * 60.0 * (1.0 + tMul + wMul);

  cout << "Vehicle : " << vType << " | Traffic: " << gtCond(adm.trData, adm.trCnt, h, "Normal") << " (+" << (int)(tMul * 100) << "%)\n"
       << "Weather : " << gtCond(adm.wtData, adm.wtCnt, h, "Clear") << " (+" << (int)(wMul * 100) << "%) | Est.Time: " << estTime << " mins\n"
       << "Fare    : " << fare << " taka\n";

  double bal = rm.bal(u);
  if (bal < fare) {
    cout << "Insufficient wallet balance (" << bal << " taka). Fare: " << fare << " taka. Ride canceled.\n";
    return;
  }

  int bestIdx = mtchdrv(from, vType);
  if (bestIdx < 0) {
    cout << "No available driver for " << vType << " right now.\n";
    return;
  }
  DriverInfo *bestDr = &dm.drivers[bestIdx];
  cout << "\nDriver Found: " << bestDr->username << " | Rating: " << bestDr->avgRating
       << " | Location: " << bestDr->location << "\n";

  int conf = inp("\nConfirm ride? (1=Yes, 2=Cancel): ", 1, 2);
  if (conf != 1) {
    cout << "Ride canceled.\n";
    return;
  }

  // Driver BFS diye shortest path khuje ashtese
  int dCityIdx = cg.fndcty(bestDr->location);
  Vector<int> arrPath; double arrDist = 0.0;
  if (dCityIdx >= 0 && fIdx >= 0) cg.bfs(dCityIdx, fIdx, arrPath, arrDist);

  cout << "\n[Driver Arriving via BFS Path]: ";
  if (!arrPath.emp()) {
    for (int k = 0; k < arrPath.sz(); k++) {
      cout << cg.cities[arrPath[k]];
      if (k < arrPath.sz() - 1) cout << " -> "; else cout << "\n";
    }
    cout << "Driver " << bestDr->username << " arrived at pickup (" << from << ")!\n";
  }

  rm.ddcFare(u, fare);
  dm.addern(bestDr->username, pm.drvShr(fare));
  dm.stlc(bestDr->username, to);
  pm.addRev(fare);

  RideRecord re;
  re.timestamp = now(); re.user = u; re.driver = bestDr->username;
  re.from = from; re.to = to; re.distance = dist; re.fare = fare;
  re.vehicle = vType;
  addRide(re);

  int totalMins = max(1, (int)estTime);

  system("cls");
  cout << "============ RIDE IN PROGRESS ============\n";
  cout << "Vehicle: " << vType << " | Driver: " << bestDr->username << "\n";
  cout << "Trip   : " << from << " -> " << to << " (" << dist << " km)\n";
  cout << "------------------------------------------\n";

  for (int m = totalMins; m >= 0; m--) {
    cout << "\r[" << m << " min(s) left] ";
    if (!path.emp()) {
      int nodeIdx = (int)(((double)(totalMins - m) / totalMins) * (path.sz() - 1));
      for (int k = 0; k <= nodeIdx; k++) {
        cout << cg.cities[path[k]];
        if (k < nodeIdx) cout << " -> ";
      }
    }
    cout << flush;
    Sleep(300);
  }
  cout << "\n";

  cout << "\nTrip completed successfully! Total Fare: " << fare << " taka\n";
  int rat = inp("Rate your driver (1-5): ", 1, 5);
  dm.rte(bestDr->username, rat);
  cout << "Thank you for rating!\n";
  cout << "==========================================\n";

  rm.sv("users.csv");
  dm.svfl("drivers.csv");
}

void rdrMnu(string u) {
  while (true) {
    system("cls");
    cout << "\n========================================\n";
    cout << "  Ride Sharing System - User Menu (" << u << ")\n";
    cout << "========================================\n";
    cout << "1. Book a Ride\n";
    cout << "2. Trip History (Stack - Recent First)\n";
    cout << "3. Add Money to Wallet\n";
    cout << "4. Check Wallet Balance\n";
    cout << "5. Favorite Locations (Singly LinkedList)\n";
    cout << "6. Logout\n";

    int ch = inp("Choice (1-6): ", 1, 6);

    switch (ch) {
    case 1:
      bokRde(u);
      break;
    case 2:
      shwHist(u, false);
      break;
    case 3: {
      double amt = inp("Amount to add: ", 1.0);
      pm.addrq(u, amt);
      cout << "Wallet request submitted. Waiting for admin approval.\n";
      pm.svRq("wallet_requests.csv");
      break;
    }
    case 4:
      cout << "Wallet Balance: " << rm.bal(u) << " taka\n";
      break;
    case 5: {
      cout << "\n--- FAVORITE LOCATIONS (Singly LinkedList) ---\n";
      RiderInfo *r = rm.gtrdr(u);
      if (r) {
        cout << "1. View Favorites\n";
        cout << "2. Add Favorite Location\n";
        cout << "3. Remove Favorite Location\n";
        int fCh = inp("Choice (1-3): ", 1, 3);
        if (fCh == 2) {
          string fav = selLoc("");
          r->favorites.insrtTl(fav);
          rm.sv("users.csv");
          cout << "Location '" << fav << "' added to favorites (Singly LinkedList)!\n";
        } else if (fCh == 3) {
          if (r->favorites.emp()) {
            cout << "No favorite locations to remove.\n";
          } else {
            cout << "Favorites:\n";
            for (int i = 0; i < r->favorites.sz(); ++i) {
              cout << "  " << i + 1 << ". " << r->favorites.get(i) << "\n";
            }
            int removeChoice = inp("Choose favorite number to remove: ", 1, r->favorites.sz());
            string removed = r->favorites.get(removeChoice - 1);
            if (r->favorites.delNode(removed)) {
              rm.sv("users.csv");
              cout << "Location '" << removed << "' removed from favorites.\n";
            }
          }
        } else {
          if (r->favorites.emp()) cout << "No favorite locations added yet.\n";
          else {
            cout << "Favorites:\n";
            for (int i = 0; i < r->favorites.sz(); ++i) {
              cout << "  " << i + 1 << ". " << r->favorites.get(i) << "\n";
            }
          }
        }
      }
      break;
    }
    case 6:
      cout << "Logged out.\n";
      return;
    default:
      cout << "Invalid choice.\n";
    }

    paus();
  }
}

void drvMnu(string u) {
  while (true) {
    system("cls");
    DriverInfo *d = dm.gtdr(u);
    cout << "\n========================================\n";
    cout << "  Ride Sharing System - Driver Menu (" << u << ")\n";
    cout << "  Status  : " << (d->available ? "ONLINE" : "OFFLINE") << "\n";
    cout << "  Location: " << d->location << "\n";
    cout << "========================================\n";
    cout << "1. View Profile & Earnings\n";
    cout << "2. Toggle Online/Offline Status\n";
    cout << "3. Completed Rides History (Stack LIFO)\n";
    cout << "4. Update Current Location\n";
    cout << "5. Logout\n";

    int ch = inp("Choice (1-5): ", 1, 5);
    switch (ch) {
    case 1:
      dm.shwsts(u);
      break;
    case 2:
      dm.tglavlblty(u);
      d = dm.gtdr(u);
      cout << "You are now " << (d->available ? "ONLINE" : "OFFLINE") << ".\n";
      dm.svfl("drivers.csv");
      break;
    case 3:
      shwHist(u, true);
      break;
    case 4: {
      string newLocation = selLoc("");
      dm.stlc(u, newLocation);
      dm.svfl("drivers.csv");
      cout << "Location updated successfully.\n";
      break;
    }
    case 5:
      cout << "Logged out.\n";
      return;
    default:
      cout << "Invalid choice.\n";
    }

    paus();
  }
}

void init() {
  cg.ldnodes("connected_nodes.csv");
  rm.ld("users.csv");
  dm.ld("drivers.csv");
  adm.bldBST(rm);
  ldProf("traffic.csv", adm.trData, adm.trCnt);
  ldProf("weather.csv", adm.wtData, adm.wtCnt);
  pm.ldRq("wallet_requests.csv");
  pm.ldPrc("pricing_config.csv");
  pm.ldRev();
}

void svAll() {
  rm.sv("users.csv");
  dm.svfl("drivers.csv");
  cg.svnodes("connected_nodes.csv");
  AdminModule::svProf("traffic.csv", adm.trData, adm.trCnt);
  AdminModule::svProf("weather.csv", adm.wtData, adm.wtCnt);
  pm.svRq("wallet_requests.csv");
  pm.svPrc("pricing_config.csv");
}

int main() {
  init();

  while (true) {
      system("cls");
      cout << "\n============================================\n";
      cout << "            Ride Sharing System\n";
      cout << "              Dhaka City Based\n";
      cout << "============================================\n";
      cout << "1. User Login\n";
      cout << "2. Driver Login\n";
      cout << "3. User Sign Up\n";
      cout << "4. Driver Sign Up\n";
      cout << "5. Admin Login\n";
      cout << "6. Exit\n";
      cout << "============================================\n";

      cout << "Choice (1-6): ";
      int ch = inp("", 1, 6);

      switch (ch) {
      case 1: {
        string u = inp("Username: ");
        string p = inp("Password: ");
        if (auth.lgn(u, p) == RIDER) {
          cout << "Login successful! Welcome, " << u << ".\n";
          rdrMnu(u);
        } else cout << "Invalid user credentials.\n";
        break;
      }
      case 2: {
        string u = inp("Username: ");
        string p = inp("Password: ");
        if (auth.lgn(u, p) == DRIVER) {
          cout << "Login successful! Welcome, Driver " << u << ".\n";
          drvMnu(u);
        } else cout << "Invalid driver credentials.\n";
        break;
      }
      case 3: {
        string u = inp("New username: ");
        string p = inp("New password: ");
        if (auth.hsUsr(u)) cout << "Username already taken.\n";
        else {
          rm.addrdr(u, p);
          rm.sv("users.csv");
          RiderInfo* newRider = rm.gtrdr(u);
          if (newRider) adm.insrtBST(newRider->id, newRider->username);
          cout << "Registration successful!\n";
        }
        break;
      }
      case 4: {
        string u = inp("New driver username: ");
        string p = inp("New password: ");
        if (auth.hsUsr(u)) { cout << "Username already taken.\n"; break; }
        cout << "Vehicle types:\n";
        cout << "1. Bike\n";
        cout << "2. CNG\n";
        cout << "3. UberX\n";
        int vCh = inp("Choose vehicle (1-3): ", 1, 3);
        string vType = (vCh == 1 ? "Bike" : vCh == 2 ? "CNG" : "UberX");

        string loc = selLoc("");

        dm.adddrv(u, p, vType, loc);
        dm.svfl("drivers.csv");
        cout << "Driver registered!\n";
        break;
      }
      case 5: {
        string u = inp("Admin username: ");
        string p = inp("Admin password: ");
        if (u == "admin" && p == "123456") {
          cout << "Admin lgn successful!\n";
          adm.admnMnu(rm, dm, cg, pm);
        } else cout << "Invalid admin credentials.\n";
        break;
      }
      case 6:
        cout << "Saving all data...\n";
        svAll();
        cout << "Thanks for using RideSharing System! Goodbye.\n";
        return 0;

      default:
        cout << "Invalid choice. Please try again.\n";
      }

      paus();
    }
  return 0;
}
