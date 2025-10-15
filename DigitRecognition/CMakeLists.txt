# CMakeLists.txt for DigitRecognition project
cmake_minimum_required(VERSION 3.16)

# Project name and version
project(DigitRecognition VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Link libraries (add any external libraries you need)
target_link_libraries(${PROJECT_NAME} PRIVATE
    activations
    # Add other libraries here, e.g., pthread, opencv, etc.
)

# Installation rules (optional)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)