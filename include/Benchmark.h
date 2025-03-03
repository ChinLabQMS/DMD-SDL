#include <iostream>
#include <chrono>

template <typename ClassType, typename ReturnType, typename... Args>
auto measureExecutionTime(const std::string& label, ReturnType (ClassType::*func)(Args...), ClassType& obj, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();

    if constexpr (std::is_void_v<ReturnType>) {
        (obj.*func)(std::forward<Args>(args)...); // Just call the function
    } else {
        ReturnType result = (obj.*func)(std::forward<Args>(args)...); // Capture return value
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << label << " execution time: " << elapsed.count() * 1000 << " milliseconds" << std::endl;
        return result;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << label << " execution time: " << elapsed.count() * 1000 << " milliseconds" << std::endl;
}

template <typename ReturnType, typename... Args>
auto measureExecutionTime(const std::string& label, ReturnType (*func)(Args...), Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();

    // Call the function
    if constexpr (std::is_void_v<ReturnType>) {
        func(std::forward<Args>(args)...);
    } else {
        ReturnType result = func(std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << label << " execution time: " << elapsed.count() * 1000 << " miliseconds" << std::endl;
        return result;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << label << " execution time: " << elapsed.count() * 1000 << " miliseconds" << std::endl;
}