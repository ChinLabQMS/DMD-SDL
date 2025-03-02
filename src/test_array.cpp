#include "Benchmark.h"
#include <vector>
#include <numeric>   // For std::iota
#include <execution> // For parallel execution policies (C++17+)
#include <omp.h>     // OpenMP

void std_iota(std::vector<uint32_t>& vec) {
    std::iota(vec.begin(), vec.end(), 0);
}

void for_loop(std::vector<uint32_t>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i] = i;
    }
}

void openmp(std::vector<uint32_t>& vec) {
    #pragma omp parallel for
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i] = i;
    }
}

int main() {
    std::vector<uint32_t> vec(1000000);

    measureExecutionTime("std::iota", std_iota, vec);
    measureExecutionTime("for loop", for_loop, vec);
    measureExecutionTime("OpenMP", openmp, vec);

    measureExecutionTime("std::iota", std_iota, vec);
    measureExecutionTime("for loop", for_loop, vec);
    measureExecutionTime("OpenMP", openmp, vec);

    measureExecutionTime("std::iota", std_iota, vec);
    measureExecutionTime("for loop", for_loop, vec);
    measureExecutionTime("OpenMP", openmp, vec);

    return 0;
}
