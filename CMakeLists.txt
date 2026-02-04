cmake_minimum_required(VERSION 3.10)
project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

# Tell CMake where the header files are
include_directories(include)

# Find OpenMP
find_package(OpenMP REQUIRED)

# Point to the files inside the src/ folder
add_executable(PhysicsEngine src/main.cpp src/engine.cpp)

target_link_libraries(PhysicsEngine PUBLIC OpenMP::OpenMP_CXX)