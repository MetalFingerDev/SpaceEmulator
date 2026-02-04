#include "physics.hpp"
#include <omp.h>
#include <iostream>

// Loop Work-sharing: Parallel Addition
std::vector<double> vector_add(const std::vector<double>& a, const std::vector<double>& b) {
    size_t n = a.size();
    std::vector<double> result(n);

    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}

// Loop Work-sharing: Parallel Dot Product with Reduction
double vector_dot(const std::vector<double>& a, const std::vector<double>& b) {
    double total = 0.0;
    size_t n = a.size();

    #pragma omp parallel for reduction(+:total)
    for (size_t i = 0; i < n; ++i) {
        total += a[i] * b[i];
    }
    return total;
}

// Sections Work-sharing: Running distinct tasks in parallel
void run_parallel_tasks() {
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            std::cout << "Task A (Thread " << omp_get_thread_num() << ") is calculating physics...\n";
        }
        #pragma omp section
        {
            std::cout << "Task B (Thread " << omp_get_thread_num() << ") is logging telemetry...\n";
        }
    }
}
