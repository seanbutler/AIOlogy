# CMakeLists.txt for layers library
cmake_minimum_required(VERSION 3.16)

# Library name
set(LIBRARY_NAME layers)

# Add the library as STATIC
add_library(${LIBRARY_NAME} STATIC
    layers.cpp
    layers.h
    # Add more source files here as needed
)

# Set C++ standard for this library
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_23)

# Include directories for this library
target_include_directories(${LIBRARY_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link with the activations library
target_link_libraries(${LIBRARY_NAME} PUBLIC activations)

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

# Enable testing for this library
if(BUILD_TESTING)
    # Create tests subdirectory
    add_subdirectory(tests)
endif()