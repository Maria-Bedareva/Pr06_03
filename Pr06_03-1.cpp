#include <iostream>
#include <omp.h>
#include <thread>

using namespace std;

int main() {
    const int n = 100;
    int* arr = new int[n];

    srand(time(nullptr));
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 100 + 1;
    }

    int sum_no_parallel = 0;
    int sum_parallel = 0;

    auto start_time = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        sum_no_parallel += arr[i];
    }
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();

    int num_threads = thread::hardware_concurrency();

    auto start_time_p = chrono::high_resolution_clock::now();
#pragma omp parallel for reduction(+:sum_parallel) num_threads(num_threads)
    for (int i = 0; i < n; ++i) {
        sum_parallel += arr[i];
    }
    auto end_time_p = chrono::high_resolution_clock::now();
    auto duration_p = chrono::duration_cast<chrono::nanoseconds>(end_time_p - start_time_p).count();

    cout << "Number of threads used: " << num_threads << endl;
    cout << "Sum without parallelization: " << sum_no_parallel << endl;
    cout << "Time without parallelization: " << duration << " nanoseconds" << endl;
    cout << "Sum with parallelization: " << sum_parallel << endl;
    cout << "Time with parallelization: " << duration_p << " nanoseconds" << endl;

    return 0;
}
