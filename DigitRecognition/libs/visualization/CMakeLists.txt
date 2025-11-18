# CMakeLists.txt for visualization library
cmake_minimum_required(VERSION 3.16)

# Create visualization library (static)
add_library(visualization STATIC
    visualization.cpp
    visualization.hpp
)

# Include directories
target_include_directories(visualization PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link SDL2 and SDL2_image
target_link_libraries(visualization PUBLIC
    SDL2::SDL2
    SDL2_image::SDL2_image
)

# Set C++ standard
target_compile_features(visualization PUBLIC cxx_std_23)

# Add HAS_SDL_IMAGE definition
target_compile_definitions(visualization PUBLIC HAS_SDL_IMAGE=1)

# Compiler-specific flags
if(MSVC)
    target_compile_options(visualization PRIVATE /W4)
else()
    target_compile_options(visualization PRIVATE -Wall -Wextra)
endif()
