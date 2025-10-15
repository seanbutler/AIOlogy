# CMakeLists.txt for activations library
cmake_minimum_required(VERSION 3.16)

# Library name
set(LIBRARY_NAME activations)

# Add the library
add_library(${LIBRARY_NAME}
    activations.cpp
    activations.h
    # Add more source files here as needed
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

# Enable testing for this library
if(BUILD_TESTING)
    # Create test executable
    add_executable(test_activations
        tests/test_activations.cpp
    )
    
    # Link the test executable with the library
    target_link_libraries(test_activations PRIVATE ${LIBRARY_NAME})
    
    # Set C++ standard for test
    target_compile_features(test_activations PRIVATE cxx_std_23)
    
    # Add compiler flags for tests
    if(MSVC)
        target_compile_options(test_activations PRIVATE /W4)
    else()
        target_compile_options(test_activations PRIVATE -Wall -Wextra)
    endif()
    
    # Register the test with CTest
    add_test(NAME ActivationsLibraryTest COMMAND test_activations)
    
    # Set test properties
    set_tests_properties(ActivationsLibraryTest PROPERTIES
        TIMEOUT 30
        PASS_REGULAR_EXPRESSION "All tests passed!"
    )
endif()