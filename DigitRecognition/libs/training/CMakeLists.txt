# CMakeLists.txt for training library
cmake_minimum_required(VERSION 3.16)

# Library name
set(LIBRARY_NAME training)

# Add the library as STATIC
add_library(${LIBRARY_NAME} STATIC
    training.cpp
    training.hpp
)

# Set C++ standard for this library
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_23)

# Include directories for this library
target_include_directories(${LIBRARY_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Compiler-specific flags for the library
if(MSVC)
    target_compile_options(${LIBRARY_NAME} PRIVATE /W4)
else()
    target_compile_options(${LIBRARY_NAME} PRIVATE -Wall -Wextra)
endif()

# Set library properties
set_target_properties(${LIBRARY_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)