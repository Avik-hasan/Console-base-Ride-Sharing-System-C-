#pragma once

#include <string>
#include "bst.hpp"
#include "rider.hpp"
#include "driver.hpp"
#include "graph.hpp"
#include "payment.hpp"

class Profile {
public:
    int hour;
    std::string cond;
    double mul;
};

class AdminModule {
private:
    UserAccountBST userBST; // BST userID khujar jonnno

    void ctyMnu(CityGraph& g);
    void usrMnu(RiderModule& rm);
    void drvMnu(DriverModule& dm);
    void prceMnu(PaymentSystem& pm);
    void wlltMnu(PaymentSystem& pm, RiderModule& rm);

    void fndBST(RiderModule& rm, int id);
    void fndDrvBS(DriverModule& dm, int id);
    void dashbrd(RiderModule& rm, DriverModule& dm, PaymentSystem& pm, CityGraph* g = nullptr);

    void addCty(CityGraph& g);
    void rmvCty(CityGraph& g);
    void addRd(CityGraph& g);
    void rmvRd(CityGraph& g);
    void vwMap(CityGraph& g);

    void shwDmnd(CityGraph& g, std::string fn);

public:
    Profile trData[24];
    Profile wtData[24];
    int trCnt;
    int wtCnt;

    AdminModule();

    static void svProf(const std::string& fn, const Profile data[], int cnt);

    void bldBST(RiderModule& rm);
    void insrtBST(int id, const std::string& username);

    void admnMnu(RiderModule& rm, DriverModule& dm, CityGraph& g, PaymentSystem& pm);
};
