#include "sorting.hpp"
#include "driver.hpp"
#include <algorithm>

using namespace std;

static bool better(const DriverInfo& a, const DriverInfo& b) {
    return (a.avgRating != b.avgRating) ? (a.avgRating > b.avgRating) : (a.tripCount > b.tripCount);
}

// Merge Sort Driver Leaderboard er jonno
static void mrgdrv(int* arr, int left, int mid, int right, const DriverInfo* drivers) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int* leftArr = new int[n1];
    int* rightArr = new int[n2];

    for (int i = 0; i < n1; i++) leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++) rightArr[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (better(drivers[leftArr[i]], drivers[rightArr[j]])) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }
    while (i < n1) arr[k++] = leftArr[i++];
    while (j < n2) arr[k++] = rightArr[j++];

    delete[] leftArr;
    delete[] rightArr;
}

void mrgesrt(int* arr, int left, int right, const DriverInfo* drivers) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mrgesrt(arr, left, mid, drivers);
    mrgesrt(arr, mid + 1, right, drivers);
    mrgdrv(arr, left, mid, right, drivers);
}

// Selection Sort to sort view demand heatmap e use hoise
void selsort(Vector<DemandItem>& items) {
    int n = items.sz();
    for (int i = 0; i < n - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (items[j].count > items[maxIdx].count) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            swap(items[i], items[maxIdx]);
        }
    }
}
