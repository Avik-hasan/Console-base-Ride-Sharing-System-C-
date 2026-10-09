#pragma once

#include <string>
#include "vector.hpp"

// Merge Sort Driver Leaderboard er jonno
class DriverInfo;
void mrgesrt(int* arr, int left, int right, const DriverInfo* drivers);

// Demand Heatmap er item
class DemandItem {
public:
    std::string location;
    int count;
};

// Selection Sort Demand Heatmap er jonno
void selsort(Vector<DemandItem>& items);
