#include "auth.hpp"

AuthSystem::AuthSystem(RiderModule *r, DriverModule *d) : rm(r), dm(d) {}



Role AuthSystem::lgn(const string &u, const string &p) const {
  if (u == "admin" && p == "123456")
    return ADMIN;

  if (rm) {
    RiderInfo *r = rm->gtrdr(u);
    if (r && r->password == p)
      return RIDER;
  }

  if (dm) {
    DriverInfo *d = dm->gtdr(u);
    if (d && d->password == p)
      return DRIVER;
  }

  return NONE;
}

bool AuthSystem::hsUsr(const string &u) const {
  if (u == "admin")
    return true;
  if (rm && rm->fndrdr(u) != -1)
    return true;
  if (dm && dm->fnddrv(u) != -1)
    return true;
  return false;
}
