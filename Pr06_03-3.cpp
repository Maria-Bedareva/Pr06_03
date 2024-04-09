#include <iostream>
#include <omp.h>
#include <chrono>
#include <thread>

using namespace std;

int main() {
    const int points = 10000000; 
    int circle = 0;

    int threads = thread::hardware_concurrency();

    auto start_time = chrono::high_resolution_clock::now();

#pragma omp parallel for threads(threads) reduction(+:circle)
    for (int i = 0; i < points; ++i) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            circle++;
        }
    }

    double pi = 4.0 * circle / points;

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    cout << "Estimated value of pi: " << pi << endl;
    cout << "Time taken for calculation: " << duration << " milliseconds" << endl;

    return 0;
}
