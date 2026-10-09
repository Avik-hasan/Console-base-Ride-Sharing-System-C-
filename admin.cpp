#include "admin.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include "stack.hpp"
#include "sorting.hpp"

using namespace std;

AdminModule::AdminModule() : trCnt(0), wtCnt(0) {}

void AdminModule::bldBST(RiderModule &rm) {
  for (int i = 0; i < rm.riders.sz(); i++) {
    userBST.insrt(rm.riders[i].id, rm.riders[i].username);
  }
}

void AdminModule::insrtBST(int id, const string& username) {
  userBST.insrt(id, username);
}

void AdminModule::fndBST(RiderModule &rm, int id) {
  string *res = userBST.srch(id);
  RiderInfo *r = res ? rm.gtrdr(*res) : nullptr;
  if (r) {
    cout << "[BST Search] ID: " << r->id << " | User: " << r->username 
         << " | Wallet: " << r->wallet << " taka\n";
  } else {
    cout << "User with ID " << id << " not found.\n";
  }
}

void AdminModule::fndDrvBS(DriverModule &dm, int id) {
  DriverInfo *d = dm.gtdrID(id);
  if (d) {
    cout << "[Recursive Binary Search] ID: " << d->id << " | User: " << d->username << " | Vehicle: " << d->vehicleType 
         << " | City: " << d->location << " | " << (d->available ? "ONLINE" : "OFFLINE") 
         << " | Rating: " << d->avgRating << "\n";
  } else {
    cout << "Driver with ID " << id << " not found.\n";
  }
}

static void srtDmnd(CityGraph* g, const string& fn, Vector<DemandItem>& items) {
  items.clr();
  ifstream file(fn);
  if (!file.is_open()) return;

  int nCities = g ? g->cities.sz() : 0;
  for (int i = 0; i < nCities; i++) {
    DemandItem it;
    it.location = g->cities[i];
    it.count = 0;
    items.pshbk(it);
  }

  string line;
  while (getline(file, line)) {
    if (line.empty()) continue;
    stringstream ss(line);
    string p[8];
    for (int i = 0; i < 8 && getline(ss, p[i], ','); i++);
    for (int i = 0; i < items.sz(); i++) {
      if (items[i].location == p[3] || items[i].location == p[4]) items[i].count++;
    }
  }

  selsort(items);
}

void AdminModule::dashbrd(RiderModule &rm, DriverModule &dm, PaymentSystem &pm, CityGraph* g) {
  Vector<DemandItem> items;
  srtDmnd(g, "rides.csv", items);

  string topArea = (!items.emp() && items[0].count > 0) ? items[0].location : "N/A";
  int topCount = (!items.emp()) ? items[0].count : 0;

  cout << "\n=== DASHBOARD ===\n"
       << "Riders: " << rm.riders.sz() 
       << " | Drivers: " << dm.drivers.sz() 
       << " | Pending Top-ups: " << pm.pndng() << "\n"
       << "Company Earnings : " << pm.companyRevenue << " BDT\n"
       << "Top Demand Area  : " << topArea << " (" << topCount << " requests)\n"
       << "=================\n";
}



// input vul jeno na ashe tai function overloading kore handle
static string inp(const string& prompt = "") {
  if (!prompt.empty()) cout << prompt;
  string s;
  while (getline(cin, s)) {
    if (!s.empty()) return s;
    if (!prompt.empty()) cout << prompt;
  }
  return "";
}

static int inp(const string& prompt, int minVal, int maxVal) {
  while (true) {
    try {
      int val = stoi(inp(prompt));
      if (val >= minVal && val <= maxVal) return val;
    } catch (...) {}
    cout << "Invalid choice! Enter (" << minVal << "-" << maxVal << "): ";
  }
}

static double inp(const string& prompt, double minVal) {
  while (true) {
    try {
      double val = stod(inp(prompt));
      if (val >= minVal) return val;
    } catch (...) {}
    cout << "Invalid input! Enter positive number: ";
  }
}

// Select two different cities helper
static bool sel2cty(CityGraph& g, int& i1, int& i2) {
  if (g.cities.sz() < 2) {
    cout << "Need at least 2 cities in graph.\n";
    return false;
  }
  g.shwcty();
  int n = g.cities.sz();
  int num1 = inp("Select City 1 (1-" + to_string(n) + "): ", 1, n);
  int num2 = inp("Select City 2 (1-" + to_string(n) + "): ", 1, n);
  if (num1 == num2) {
    cout << "Both cities cannot be the same!\n";
    return false;
  }
  i1 = num1 - 1;
  i2 = num2 - 1;
  return true;
}

void AdminModule::svProf(const string& fn, const Profile data[], int cnt) {
  ofstream fout(fn);
  if (!fout.is_open()) return;
  for (int i = 0; i < cnt; i++) {
    if (!data[i].cond.empty()) {
      fout << data[i].hour << "," << data[i].cond << "," << data[i].mul << "\n";
    }
  }
}

void AdminModule::addCty(CityGraph &g) {
  string name = inp("Enter new city name: ");
  if (g.fndcty(name) >= 0) {
    cout << "City '" << name << "' already exists!\n";
    return;
  }
  g.addcty(name);
  g.svnodes("connected_nodes.csv");
  cout << "City '" << name << "' added successfully to connected_nodes.csv.\n";
}

void AdminModule::rmvCty(CityGraph &g) {
  if (g.cities.emp()) {
    cout << "No cities in graph.\n";
    return;
  }
  g.shwcty();
  int num = inp("Enter city number to remove (1-" + to_string(g.cities.sz()) + "): ", 1, g.cities.sz());
  string cityName = g.cities[num - 1];
  if (g.rmvcty(num - 1)) {
    g.svnodes("connected_nodes.csv");
    cout << "City '" << cityName << "' and its connected roads removed successfully.\n";
  } else {
    cout << "Failed to remove city.\n";
  }
}

void AdminModule::addRd(CityGraph &g) {
  int i1, i2;
  if (!sel2cty(g, i1, i2)) return;
  double w = inp("Enter road distance (km): ", 0.01);
  g.addrd(i1, i2, w);
  g.svnodes("connected_nodes.csv");
  cout << "Road added between " << g.cities[i1] << " & " << g.cities[i2] << " (" << w << " km) and saved.\n";
}

void AdminModule::rmvRd(CityGraph &g) {
  int i1, i2;
  if (!sel2cty(g, i1, i2)) return;
  if (g.hsrd(i1, i2)) {
    g.rmvrd(i1, i2);
    g.svnodes("connected_nodes.csv");
    cout << "Road removed between " << g.cities[i1] << " & " << g.cities[i2] << " and saved.\n";
  } else {
    cout << "No road exists between these two cities.\n";
  }
}

void AdminModule::vwMap(CityGraph &g) {
  cout << "\nTotal cities in graph: " << g.cities.sz() << "\n";
  ifstream fin("connected_nodes.csv");
  if (!fin) {
    cout << "Road network file not found.\n";
    return;
  }
  cout << "\n========== DHAKA ROAD NETWORK (Adjacency Connections) ==========\n";
  string line;
  int count = 1;
  while (getline(fin, line)) {
    if (line.empty()) continue;
    stringstream ss(line);
    string src, dest, dist;
    getline(ss, src, ',');
    cout << count++ << ". " << src << " -> ";
    bool first = true;
    while (getline(ss, dest, ',') && getline(ss, dist, ',')) {
      if (!first) cout << " | ";
      cout << dest << " (" << dist << " km)";
      first = false;
    }
    cout << "\n";
  }
  cout << "=================================================================\n";
}

void AdminModule::shwDmnd(CityGraph &g, string fn) {
  Vector<DemandItem> items;
  srtDmnd(&g, fn, items);
  if (items.emp()) { cout << "No ride records.\n"; return; }

  cout << "\n--- DEMAND HEATMAP (Sorted via Selection Sort) ---\n";
  for (int i = 0; i < items.sz(); i++) {
    if (items[i].count > 0) {
      cout << i + 1 << ". " << items[i].location << " | Requests: " << items[i].count << "\n";
    }
  }
}



static void edtProf(const string& typeName, const string& fileName, Profile data[], int& cnt) {
  cout << "\n--- " << typeName << " PROFILE ---\n1. View Profile\n2. Add / Update Hour Surge\n";
  int ch = inp("Choice (1-2): ", 1, 2);
  if (ch == 1) {
    if (cnt == 0) cout << "No custom profiles.\n";
    for (int i = 0; i < cnt; i++) {
      if (!data[i].cond.empty()) {
        cout << data[i].hour << ":00 | " << data[i].cond << " | +" << (int)(data[i].mul * 100) << "%\n";
      }
    }
  } else {
    int h = inp("Enter hour (0-23): ", 0, 23);
    string inputLine = inp("Enter condition and surge decimal (e.g. Rain/Mid 0.20): ");
    stringstream ss(inputLine);
    string cond; double mul = 0.0;
    if (ss >> cond >> mul) {
      bool found = false;
      for (int i = 0; i < cnt; i++) {
        if (data[i].hour == h) {
          data[i].cond = cond; data[i].mul = mul;
          found = true;
          cout << "Hour " << h << ":00 updated successfully.\n"; break;
        }
      }
      if (!found && cnt < 24) {
        data[cnt].hour = h; data[cnt].cond = cond; data[cnt].mul = mul; cnt++;
        cout << "Hour " << h << ":00 added successfully.\n";
      }
      AdminModule::svProf(fileName, data, cnt);
      cout << "Profile saved to " << fileName << ".\n";
    } else {
      cout << "Invalid format! Update aborted.\n";
    }
  }
}



void AdminModule::ctyMnu(CityGraph& g) {
  cout << "1. Add City\n2. Remove City\n3. Add Road\n4. Remove Road\n5. View Map\n";
  int ch = inp("Choice (1-5): ", 1, 5);
  if (ch == 1) addCty(g);
  else if (ch == 2) rmvCty(g);
  else if (ch == 3) addRd(g);
  else if (ch == 4) rmvRd(g);
  else if (ch == 5) vwMap(g);
}

void AdminModule::usrMnu(RiderModule& rm) {
  cout << "1. List Users\n2. Search User (BST)\n3. Delete User\n";
  int ch = inp("Choice (1-3): ", 1, 3);
  if (ch == 1) rm.shwAll();
  else if (ch == 2) {
    fndBST(rm, inp("Enter ID: ", 1, 1000000));
  } else {
    int id = inp("Enter user ID: ", 1, 1000000);
    string* u = userBST.srch(id);
    if (u) {
      if (rm.rmvrdr(*u)) {
        userBST.rmv(id);
        cout << "User '" << *u << "' (ID: " << id << ") removed.\n";
      }
    } else cout << "User with ID " << id << " not found.\n";
  }
}

void AdminModule::drvMnu(DriverModule& dm) {
  cout << "1. List Drivers\n2. Search Driver (Binary Search)\n3. Delete Driver\n4. Driver Leaderboard (Merge Sort)\n";
  int ch = inp("Choice (1-4): ", 1, 4);
  if (ch == 1) dm.shwall();
  else if (ch == 2) {
    fndDrvBS(dm, inp("Enter ID: ", 1, 1000000));
  } else if (ch == 3) {
    int id = inp("Enter driver ID: ", 1, 1000000);
    DriverInfo* d = dm.gtdrID(id);
    if (d) {
      if (dm.rmvdrv(d->username)) cout << "Driver '" << d->username << "' (ID: " << d->id << ") removed.\n";
    } else cout << "Driver with ID " << id << " not found.\n";
  } else {
    cout << "\n========== DRIVER LEADERBOARD (Sorted via Merge Sort) ==========\n";
    Vector<int> idxs;
    dm.ldrbrd(idxs, 100);
    for (int i = 0; i < idxs.sz(); i++) {
      DriverInfo &dr = dm.drivers[idxs[i]];
      cout << i + 1 << ". " << dr.username << " | Vehicle: " << dr.vehicleType
           << " | Rating: " << dr.avgRating << " | Trips: " << dr.tripCount
           << " | Earnings: " << dr.totalEarnings << " BDT\n";
    }
    cout << "=================================================================\n";
  }
}

void AdminModule::prceMnu(PaymentSystem& pm) {
  pm.shwPrc();
  cout << "\n1. Vehicle Pricing\n2. Demand Surge Weight\n3. Surge Cap\n4. Back\n";
  int opt = inp("Choice (1-4): ", 1, 4);
  if (opt == 1) {
    cout << "1. Bike\n2. CNG\n3. UberX\n";
    int vOpt = inp("Select vehicle (1-3): ", 1, 3);
    string v = (vOpt == 1 ? "Bike" : vOpt == 2 ? "CNG" : "UberX");
    double b = inp("Base Fare: ", 0.0);
    double p = inp("Per Km Fare: ", 0.0);
    pm.updPrc(v, b, p);
  } else if (opt == 2) {
    pm.demandWeight = inp("Demand weight: ", 0.0);
    cout << "Updated.\n";
  } else if (opt == 3) {
    pm.maxSurgeCap = inp("Surge cap: ", 0.0);
    cout << "Updated.\n";
  }

  if (opt >= 1 && opt <= 3) {
    pm.svPrc("pricing_config.csv");
    cout << "Pricing configuration saved to pricing_config.csv\n";
  }
}

void AdminModule::wlltMnu(PaymentSystem& pm, RiderModule& rm) {
  while (true) {
    if (pm.pndng() == 0) { cout << "\nNo pending requests.\n"; break; }
    cout << "\n--- PENDING WALLET REQUESTS ---\n";
    pm.shwPnd();
    cout << "\n1. Approve\n2. Reject\n3. Back\n";
    int opt = inp("Choice (1-3): ", 1, 3);
    if (opt == 1) pm.apprv(&rm);
    else if (opt == 2) pm.rjct();
    else break;
  }
}

void AdminModule::admnMnu(RiderModule &rm, DriverModule &dm, CityGraph &g, PaymentSystem &pm) {
  int ch;
  do {
    system("cls");
    cout << "\n===== ADMIN MENU =====\n"
         << "1. Dashboard\n2. Cities & Roads\n3. Traffic Profile\n4. Weather Profile\n"
         << "5. Users (Search/Delete)\n6. Drivers (Search/Delete)\n7. Pricing\n"
         << "8. Wallet Top-up Queue\n9. Demand Heatmap\n10. All Rides Log\n11. Logout\n";
    ch = inp("Choice (1-11): ", 1, 11);

    switch (ch) {
      case 1: dashbrd(rm, dm, pm, &g); break;
      case 2: ctyMnu(g); break;
      case 3: edtProf("TRAFFIC", "traffic.csv", trData, trCnt); break;
      case 4: edtProf("WEATHER", "weather.csv", wtData, wtCnt); break;
      case 5: usrMnu(rm); break;
      case 6: drvMnu(dm); break;
      case 7: prceMnu(pm); break;
      case 8: wlltMnu(pm, rm); break;
      case 9: shwDmnd(g, "rides.csv"); break;
      case 10: cout << "\nDisplaying trips from rides.csv:\n"; shwHist(); break;
      case 11: return;
      default: cout << "Invalid choice!\n";
    }

    if (ch != 11) {
      cout << "\nPress Enter to go back...";
      string dummy;
      getline(cin, dummy);
    }
  } while (ch != 11);
}
