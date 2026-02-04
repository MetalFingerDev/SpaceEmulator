#include "physics.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

// Basic test: momentum conservation for isolated two-body system
int main() {
    std::vector<Body> bodies = {
        {-0.5, 0.0, 0.0, 0.0,  1.0, 0.0, 1.0},
        { 0.5, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0}
    };

    auto total_momentum = [&](const std::vector<Body>& b) {
        double px = 0.0, py = 0.0, pz = 0.0;
        for (auto &bb : b) {
            px += bb.mass * bb.vx;
            py += bb.mass * bb.vy;
            pz += bb.mass * bb.vz;
        }
        return std::array<double,3>{px,py,pz};
    };

    auto before = total_momentum(bodies);

    // integrate a few steps
    for (int i = 0; i < 100; ++i) {
        simulate_gravity_step(bodies, 0.01);
    }

    auto after = total_momentum(bodies);

    double eps = 1e-9;
    if (std::fabs(before[0] - after[0]) > eps || std::fabs(before[1] - after[1]) > eps || std::fabs(before[2] - after[2]) > eps) {
        std::cerr << "Momentum not conserved: before=(" << before[0] << "," << before[1] << "," << before[2]
                  << ") after=(" << after[0] << "," << after[1] << "," << after[2] << ")\n";
        return EXIT_FAILURE;
    }

    std::cout << "gravity_test: OK\n";
    return EXIT_SUCCESS;
}
