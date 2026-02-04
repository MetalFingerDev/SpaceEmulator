#include "physics.hpp"
#include <iostream>
#include <format>
#include <fstream>
#include <string>
#include <random>

int main(int argc, char** argv) {
    // Basic tests
    run_parallel_tasks();
    double dot = vector_dot({1.0,2.0,3.0,4.0}, {5.0,6.0,7.0,8.0});
    std::cout << "Dot Product: " << dot << std::endl;

    // Command-line options (simple parsing)
    std::string csv_path;
    std::string scenario = "two-body";
    int steps = 500;
    double dt = 10.0; // seconds per step
    int sample = 1;   // sample every step
    int rand_n = 5;   // for random scenario
    unsigned int seed = 42;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--csv" && (i+1) < argc) csv_path = argv[++i];
        else if (a == "--scenario" && (i+1) < argc) scenario = argv[++i];
        else if (a == "--steps" && (i+1) < argc) steps = std::stoi(argv[++i]);
        else if (a == "--dt" && (i+1) < argc) dt = std::stod(argv[++i]);
        else if (a == "--sample" && (i+1) < argc) sample = std::stoi(argv[++i]);
        else if (a == "--n" && (i+1) < argc) rand_n = std::stoi(argv[++i]);
        else if (a == "--seed" && (i+1) < argc) seed = static_cast<unsigned int>(std::stoul(argv[++i]));
    }

    std::vector<Body> bodies;
    if (scenario == "two-body") {
        bodies = {
            {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.972e24},
            {4.0e7, 0.0, 0.0, 0.0, 1200.0, 0.0, 1.0e3}
        };
    } else if (scenario == "three-body") {
        // a simple symmetric three-body setup
        bodies = {
            {-1.0e7, 0.0, 0.0, 0.0, -500.0, 0.0, 5.972e23},
            {1.0e7, 0.0, 0.0, 0.0, 500.0, 0.0, 5.972e23},
            {0.0, 2.0e7, 0.0, -400.0, 0.0, 0.0, 1.0e23}
        };
    } else if (scenario == "random") {
        std::mt19937_64 rng(seed);
        std::uniform_real_distribution<double> pos(-1.0e7, 1.0e7);
        std::uniform_real_distribution<double> vel(-1000.0, 1000.0);
        std::uniform_real_distribution<double> mass(1e20, 1e25);
        for (int i = 0; i < rand_n; ++i) {
            bodies.emplace_back( Body{pos(rng), pos(rng), 0.0, vel(rng), vel(rng), 0.0, mass(rng)} );
        }
    } else {
        std::cerr << "Unknown scenario '" << scenario << "'\n";
        return 1;
    }

    std::ofstream csv;
    if (!csv_path.empty()) {
        csv.open(csv_path);
        csv << "step,body,x,y,vx,vy\n";
    }

    for (int step = 0; step < steps; ++step) {
        simulate_gravity_step(bodies, dt);
        if (step % sample != 0) continue;
        if (csv.is_open()) {
            for (size_t i = 0; i < bodies.size(); ++i) {
                auto &b = bodies[i];
                csv << step << "," << i << "," << b.x << "," << b.y << "," << b.vx << "," << b.vy << "\n";
            }
        }
    }
    if (csv.is_open()) csv.close();

    std::cout << "Simulation complete: wrote " << (csv_path.empty() ? "(no CSV)" : csv_path) << "\n";
    return 0;
}
