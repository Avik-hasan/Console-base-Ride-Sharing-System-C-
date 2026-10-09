#pragma once

#include "driver.hpp"
#include "rider.hpp"
#include <string>

using namespace std;

enum Role { RIDER, DRIVER, ADMIN, NONE };

class AuthSystem {
private:
  RiderModule *rm;
  DriverModule *dm;

public:
  AuthSystem(RiderModule *r = nullptr, DriverModule *d = nullptr);


  Role lgn(const string &u, const string &p) const;
  bool hsUsr(const string &u) const;
};
