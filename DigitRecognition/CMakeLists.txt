# CMakeLists.txt for DigitRecognition project
cmake_minimum_required(VERSION 3.16)


# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Project name and version
project(DigitRecognition VERSION 0.1.0 LANGUAGES CXX)

# Get current date and git info
string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M:%S")
execute_process(
    COMMAND git rev-parse --short HEAD
    OUTPUT_VARIABLE GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

# Generate the actual header file
configure_file(
    "${CMAKE_SOURCE_DIR}/version.h.in"
    "${CMAKE_SOURCE_DIR}/version.h"
    @ONLY
)

# Make sure your targets can find it
# target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})



# Add compile options for better debugging and warnings
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /W4")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

# Create the main executable
# Add your source files here
add_executable(${PROJECT_NAME}
    main.cpp
    # Add other source files as needed
)

# Include directories (if you have header files in separate directories)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Enable testing
enable_testing()

# Set BUILD_TESTING option (can be overridden with -DBUILD_TESTING=OFF)
option(BUILD_TESTING "Build the testing tree" ON)

# Add subdirectories for libraries
add_subdirectory(libs/activations)
add_subdirectory(libs/layers)

# Link libraries (add any external libraries you need)
target_link_libraries(${PROJECT_NAME} PRIVATE
    layers
    activations
    # Add other libraries here, e.g., pthread, opencv, etc.
)

# Installation rules (optional)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)