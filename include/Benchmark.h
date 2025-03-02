#include <iostream>
#include <chrono>

// Template function to measure execution time of a non-static class member function
template <typename ClassType, typename ReturnType, typename... Args>
void measureExecutionTime(const std::string& label, ReturnType (ClassType::*func)(Args...), ClassType& obj, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();

    // Call the member function
    (obj.*func)(std::forward<Args>(args)...);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << label << " execution time: " << elapsed.count() * 1000 << " miliseconds" << std::endl;
}

template <typename ReturnType, typename... Args>
void measureExecutionTime(const std::string& label, ReturnType (*func)(Args...), Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();

    // Call the function
    func(std::forward<Args>(args)...);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << label << " execution time: " << elapsed.count() * 1000 << " miliseconds" << std::endl;
}