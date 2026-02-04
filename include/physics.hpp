#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <vector>

// Mathematical operations
std::vector<double> vector_add(const std::vector<double>& a, const std::vector<double>& b);
double vector_dot(const std::vector<double>& a, const std::vector<double>& b);

// Demonstrating OpenMP Sections
void run_parallel_tasks();

#endif
