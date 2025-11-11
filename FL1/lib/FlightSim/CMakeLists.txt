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

# Fetch Asio standalone library (header-only, no Boost dependency)
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-30-2
)
FetchContent_MakeAvailable(asio)

# Create header-only interface library
add_library(FlightSim INTERFACE)

# Add include directories
target_include_directories(FlightSim INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${asio_SOURCE_DIR}/asio/include>
)

# Link nlohmann_json
target_link_libraries(FlightSim INTERFACE nlohmann_json::nlohmann_json)

# Add Asio compile definitions (standalone mode, no Boost)
target_compile_definitions(FlightSim INTERFACE
    ASIO_STANDALONE
    ASIO_NO_DEPRECATED
    $<$<PLATFORM_ID:Windows>:_WIN32_WINNT=0x0601>  # Windows 7+
)

# Compiler warnings
if(MSVC)
    target_compile_options(FlightSim INTERFACE /W4)
    # Add Windows socket library for Asio
    target_link_libraries(FlightSim INTERFACE ws2_32 wsock32)
else()
    target_compile_options(FlightSim INTERFACE -Wall -Wextra -pedantic)
    # Add pthread for Asio on Unix-like systems
    find_package(Threads REQUIRED)
    target_link_libraries(FlightSim INTERFACE Threads::Threads)
endif()

message(STATUS "FlightSim library configured with Asio and nlohmann/json")
