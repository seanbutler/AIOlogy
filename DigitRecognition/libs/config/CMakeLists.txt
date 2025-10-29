# CMakeLists.txt for config library
cmake_minimum_required(VERSION 3.16)


# Create config library (static)
add_library(config STATIC config.cpp)

# Include directories
target_include_directories(config INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link nlohmann_json
target_link_libraries(config INTERFACE
    nlohmann_json::nlohmann_json
)

# Set C++ standard
target_compile_features(config INTERFACE cxx_std_23)