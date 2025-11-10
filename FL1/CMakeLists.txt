cmake_minimum_required(VERSION 3.14)
project(FlightSimWorkspace VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Print build info
message(STATUS "===========================================")
message(STATUS "Building Flight Simulation Workspace")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "===========================================")

# Add library
add_subdirectory(lib/FlightSim)

# Add applications
add_subdirectory(apps/cli)
