#include "physics_engine.hpp"
#include <iostream>

int main() {
    std::vector<double> v1 = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> v2 = {5.0, 6.0, 7.0, 8.0};

    // 1. Test Sections
    run_parallel_tasks();

    // 2. Test Loop Work-sharing
    double dot = vector_dot(v1, v2);
    std::cout << "Dot Product: " << dot << std::endl;

    return 0;
}