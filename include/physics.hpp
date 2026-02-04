#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <vector>

// Body representation for gravity simulation (point masses)
struct Body {
    double x, y, z;    // position
    double vx, vy, vz; // velocity
    double mass;       // mass
};

// Mathematical operations
std::vector<double> vector_add(const std::vector<double>& a, const std::vector<double>& b);
double vector_dot(const std::vector<double>& a, const std::vector<double>& b);

// OpenMP Sections
void run_parallel_tasks();

// Gravity simulation: advance bodies by dt using Newtonian gravity (G default SI)
void simulate_gravity_step(std::vector<Body>& bodies, double dt, double G = 6.67430e-11);

#endif
