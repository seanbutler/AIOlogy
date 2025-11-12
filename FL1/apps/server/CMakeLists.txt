cmake_minimum_required(VERSION 3.14)
project(FlightSimServer VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create executable
add_executable(FlightSimServer main.cpp)

# Link FlightSim library
target_link_libraries(FlightSimServer PRIVATE FlightSim)

# Set output name
set_target_properties(FlightSimServer PROPERTIES OUTPUT_NAME "FlightSimServer")

message(STATUS "FlightSimServer application configured")
