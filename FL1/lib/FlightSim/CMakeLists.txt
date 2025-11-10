cmake_minimum_required(VERSION 3.14)
project(FlightSim VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent for dependencies
include(FetchContent)

# Fetch nlohmann/json library
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Create header-only interface library
add_library(FlightSim INTERFACE)

# Add include directories
target_include_directories(FlightSim INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Link nlohmann_json
target_link_libraries(FlightSim INTERFACE nlohmann_json::nlohmann_json)

# Compiler warnings
if(MSVC)
    target_compile_options(FlightSim INTERFACE /W4)
else()
    target_compile_options(FlightSim INTERFACE -Wall -Wextra -pedantic)
endif()

message(STATUS "FlightSim library configured")
