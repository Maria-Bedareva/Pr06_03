#include <iostream>
#include <omp.h>
#include <thread>
#include <chrono>

using namespace std;

void sort(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; ++i) {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; ++j) {
        R[j] = arr[m + 1 + j];
    }

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        }
        else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

void no_parallel(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        no_parallel(arr, l, m);
        no_parallel(arr, m + 1, r);

        sort(arr, l, m, r);
    }
}

void parallel(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

#pragma omp parallel sections
        {
#pragma omp section
            parallel(arr, l, m);
#pragma omp section
            parallel(arr, m + 1, r);
        }

        sort(arr, l, m, r);
    }
}

int main() {
    const int size = 1000;
    int arr[size];
    srand(time(nullptr));
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 1000; 
    }

    int arr_n[size];
    int arr_p[size];
    copy(begin(arr), end(arr), begin(arr_n));
    copy(begin(arr), end(arr), begin(arr_p));

    auto start_time_n = chrono::high_resolution_clock::now();
    no_parallel(arr_n, 0, size - 1);
    auto end_time_n = chrono::high_resolution_clock::now();

    auto start_time_p = chrono::high_resolution_clock::now();
    parallel(arr_p, 0, size - 1);
    auto end_time_p = chrono::high_resolution_clock::now();

    auto duration_n = chrono::duration_cast<chrono::nanoseconds>(end_time_n - start_time_n).count();
    auto duration_p = chrono::duration_cast<chrono::nanoseconds>(end_time_p - start_time_p).count();

    cout << "Time without parallelization: " << duration_n << " nanoseconds" << endl;
    cout << "Time with parallelization: " << duration_p << " nanoseconds" << endl;

    return 0;
}
