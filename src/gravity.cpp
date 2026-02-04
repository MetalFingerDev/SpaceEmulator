#include "physics.hpp"
#include <omp.h>
#include <cmath>
#include <array>

// Advance the bodies by dt using Newtonian gravity (naive O(N^2) pairwise interactions).
// Uses a small softening term to avoid singularities. Accelerations are computed in a
// read-only pass over positions and masses, stored in a temporary buffer, and then
// velocities/positions are updated in a second parallel pass to avoid write races.

void simulate_gravity_step(std::vector<Body>& bodies, double dt, double G) {
    size_t n = bodies.size();
    if (n == 0) return;

    const double softening = 1e-3; // small softening length to avoid singular behavior
    std::vector<std::array<double,3>> acc(n);

    // Compute accelerations for each body (parallel over i)
    #pragma omp parallel for schedule(dynamic)
    for (size_t i = 0; i < n; ++i) {
        double ax = 0.0, ay = 0.0, az = 0.0;
        for (size_t j = 0; j < n; ++j) {
            if (i == j) continue;
            double dx = bodies[j].x - bodies[i].x;
            double dy = bodies[j].y - bodies[i].y;
            double dz = bodies[j].z - bodies[i].z;
            double r2 = dx*dx + dy*dy + dz*dz + softening*softening;
            double inv_r = 1.0 / std::sqrt(r2);
            double inv_r3 = inv_r * inv_r * inv_r;
            // acceleration contribution: G * m_j / r^3 * r_vec
            double factor = G * bodies[j].mass * inv_r3;
            ax += factor * dx;
            ay += factor * dy;
            az += factor * dz;
        }
        acc[i] = {ax, ay, az};
    }

    // Update velocities and positions (parallel-safe because each index is independent)
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        bodies[i].vx += acc[i][0] * dt;
        bodies[i].vy += acc[i][1] * dt;
        bodies[i].vz += acc[i][2] * dt;

        bodies[i].x += bodies[i].vx * dt;
        bodies[i].y += bodies[i].vy * dt;
        bodies[i].z += bodies[i].vz * dt;
    }
}
