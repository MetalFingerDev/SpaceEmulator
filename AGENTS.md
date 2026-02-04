# Agent Context: Simple Physics Engine

## Purpose

A bare-bones educational project for learning C++ parallelism using OpenMP.

## Tech Stack

- **Language**: C++17
- **Build System**: CMake
- **Parallelism**: OpenMP (find_package(OpenMP))

## Key Implementations

1. **Vector Math**: `engine.cpp` uses `#pragma omp parallel for` for vector addition and dot product (with reduction).
2. **Work-sharing**: `run_parallel_tasks()` demonstrates `#pragma omp parallel sections`.
3. **Data Structures**: Uses `std::vector<double>` for arbitrary length support.

## Building

```bash
mkdir build && cd build
cmake ..
make
./PhysicsEngine
```
