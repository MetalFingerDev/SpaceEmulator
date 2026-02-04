cmake_minimum_required(VERSION 3.10)
project(PhysicsEngine)

# Require C++20 for std::format and modern features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where the header files are
include_directories(include)

# Find OpenMP
find_package(OpenMP REQUIRED)

# Point to the files inside the src/ folder
add_executable(PhysicsEngine src/main.cpp src/engine.cpp src/gravity.cpp)

target_link_libraries(PhysicsEngine PUBLIC OpenMP::OpenMP_CXX)

# Optional testing targets
option(ENABLE_TESTING "Build and run tests" OFF)
if(ENABLE_TESTING)
  enable_testing()
  add_executable(test_gravity tests/test_gravity.cpp src/gravity.cpp src/engine.cpp)
  target_link_libraries(test_gravity PUBLIC OpenMP::OpenMP_CXX)
  add_test(NAME gravity_test COMMAND test_gravity)
endif()