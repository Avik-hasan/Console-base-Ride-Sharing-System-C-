 #include "heap.hpp"
#include <algorithm>

DriverMatchingMinHeap::DriverMatchingMinHeap() : n(0) {}

bool DriverMatchingMinHeap::emp() const {
    return n == 0;
}

void DriverMatchingMinHeap::hpFy(int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int smallest = i;

    if (left < n && (arr[left].distance < arr[smallest].distance ||
        (arr[left].distance == arr[smallest].distance && arr[left].rating > arr[smallest].rating))) {
        smallest = left;
    }

    if (right < n && (arr[right].distance < arr[smallest].distance ||
        (arr[right].distance == arr[smallest].distance && arr[right].rating > arr[smallest].rating))) {
        smallest = right;
    }

    if (smallest != i) {
        std::swap(arr[i], arr[smallest]);
        hpFy(smallest);
    }
}

void DriverMatchingMinHeap::psh(HeapItem item) {
    int i = n;
    arr[n++] = item;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (arr[i].distance < arr[parent].distance ||
           (arr[i].distance == arr[parent].distance && arr[i].rating > arr[parent].rating)) {
            std::swap(arr[parent], arr[i]);
            i = parent;
        } else {
            break;
        }
    }
}

//kacher driver tule ane    
HeapItem DriverMatchingMinHeap::extMin() {
    if (n <= 0) return HeapItem();

    HeapItem closest = arr[0];
    arr[0] = arr[n - 1];
    n--;

    hpFy(0);
    return closest;
}
