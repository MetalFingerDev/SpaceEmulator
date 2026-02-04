#include "physics.hpp"
#include <iostream>
#include <format>

int main() {
    std::vector<double> v1 = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> v2 = {5.0, 6.0, 7.0, 8.0};

    // 1. Test Sections
    run_parallel_tasks();

    // 2. Test Loop Work-sharing
    double dot = vector_dot(v1, v2);
    std::cout << "Dot Product: " << dot << std::endl;

    // 3. Test gravity (simple two-body demo)
    std::vector<Body> bodies = {
        // x, y, z, vx, vy, vz, mass
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.972e24},  // heavy body (Earth-like)
        {4.0e7, 0.0, 0.0, 0.0, 1200.0, 0.0, 1.0e3}  // small body with initial tangential velocity
    };
    double dt = 1.0; // seconds per step

    // Compact two-column table: show x (scientific), y, vx, vy; sample every 2 steps
    constexpr auto row_fmt = "{:<4} | {:>12.3e} {:>8.2f} {:>8.2f} {:>8.2f} || {:>12.3e} {:>8.2f} {:>8.2f} {:>8.2f}\n";

    constexpr auto header_title = "{:<4} | {:>39} || {:>39}\n";
    constexpr auto header_cols  = "{:<4} | {:>12} {:>8} {:>8} {:>8} || {:>12} {:>8} {:>8} {:>8}\n";

    std::cout << "\nGravity demo (sampled every 2 steps)\n";
    std::cout << std::string(89, '-') << "\n";
    std::cout << std::format(header_title, "Step", "Body0 (x y vx vy)", "Body1 (x y vx vy)");
    std::cout << std::format(header_cols, "", "x(m)", "y(m)", "vx", "vy", "x(m)", "y(m)", "vx", "vy");
    std::cout << std::string(89, '-') << "\n";

    // Optionally dump CSV if requested (usage: ./PhysicsEngine --csv out.csv)
    std::string csv_path;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--csv" && (i + 1) < argc) {
            csv_path = argv[i+1];
            break;
        }
    }

    std::ofstream csv;
    if (!csv_path.empty()) {
        csv.open(csv_path);
        csv << "step,body,x,y,vx,vy\n";
    }

    for (int step = 0; step < 10; ++step) {
        simulate_gravity_step(bodies, dt);
        if (step % 2 != 0) continue;
        auto &b0 = bodies[0];
        auto &b1 = bodies[1];
        std::cout << std::format(row_fmt, step,
                                 b0.x, b0.y, b0.vx, b0.vy,
                                 b1.x, b1.y, b1.vx, b1.vy);
        if (csv.is_open()) {
            csv << step << ",0," << b0.x << "," << b0.y << "," << b0.vx << "," << b0.vy << "\n";
            csv << step << ",1," << b1.x << "," << b1.y << "," << b1.vx << "," << b1.vy << "\n";
        }
    }
    if (csv.is_open()) csv.close();

    return 0;
}
