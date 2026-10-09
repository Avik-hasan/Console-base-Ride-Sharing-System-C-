#pragma once

class HeapItem {
public:
    int driverIndex;
    double distance;
    double rating;

    HeapItem(int idx = -1, double dist = 0.0, double rate = 0.0)
        : driverIndex(idx), distance(dist), rating(rate) {}
};

// shortest distance r highest rating r opore min heap diye driver matching
class DriverMatchingMinHeap {
private:
    HeapItem arr[1100];
    int n;

    void hpFy(int i);

public:
    DriverMatchingMinHeap();

    void psh(HeapItem item);
    HeapItem extMin();
    bool emp() const;
};
