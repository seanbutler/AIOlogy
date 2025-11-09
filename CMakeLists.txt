cmake_minimum_required(VERSION 3.14)
project(AircraftSimulation)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent module
include(FetchContent)

# Fetch nlohmann/json library
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Add executable
add_executable(aircraft_sim main.cpp)

# Link nlohmann_json library
target_link_libraries(aircraft_sim PRIVATE nlohmann_json::nlohmann_json)

# Include directories (headers are in the same directory)
target_include_directories(aircraft_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Compiler warnings
if(MSVC)
    target_compile_options(aircraft_sim PRIVATE /W4)
else()
    target_compile_options(aircraft_sim PRIVATE -Wall -Wextra -pedantic)
endif()

# Print build info
message(STATUS "Building Aircraft Simulation")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
