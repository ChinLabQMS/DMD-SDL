
#include <iostream>
#include <vector>
#include <numeric>   // For std::iota
#include <execution> // For parallel execution policies (C++17+)
#include <chrono>    // For timing
#include <omp.h>     // OpenMP

void benchmark_std_iota() {
    std::vector<int> vec(1'000'000);
    auto start = std::chrono::high_resolution_clock::now();
    
    std::iota(vec.begin(), vec.end(), 0);  // Single-threaded iota

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "std::iota (single-threaded): "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";
}

void benchmark_for() {
    std::vector<int> vec(1'000'000);
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i] = i;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "For loop: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";
}

void benchmark_openmp() {
    std::vector<int> vec(1'000'000);
    auto start = std::chrono::high_resolution_clock::now();
    
    #pragma omp parallel for
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i] = i;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "OpenMP parallel for: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";
}

int main() {
    benchmark_std_iota();
    benchmark_openmp();
    benchmark_for();
    benchmark_std_iota();
    benchmark_openmp();
    benchmark_for();
    benchmark_std_iota();
    benchmark_openmp();
    benchmark_for();

    return 0;
}
