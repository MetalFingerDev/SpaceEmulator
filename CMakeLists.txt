cmake_minimum_required(VERSION 3.10)
project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

# Find OpenMP package
find_package(OpenMP REQUIRED)

add_executable(PhysicsEngine main.cpp engine.cpp)

# Link OpenMP to the executable
target_link_libraries(PhysicsEngine PUBLIC OpenMP::OpenMP_CXX)