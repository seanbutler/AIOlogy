cmake_minimum_required(VERSION 3.14)
project(FlightSimCLI VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create executable
add_executable(FlightSimCLI main.cpp)

# Link FlightSim library
target_link_libraries(FlightSimCLI PRIVATE FlightSim)

# Set output name
set_target_properties(FlightSimCLI PROPERTIES OUTPUT_NAME "FlightSim")

message(STATUS "FlightSimCLI application configured")
