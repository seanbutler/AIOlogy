# CMakeLists.txt for layers tests
cmake_minimum_required(VERSION 3.16)

# Create test executable
add_executable(test_layers
    test_layers.cpp
)

# Link the test executable with the layers library
target_link_libraries(test_layers PRIVATE layers)

# Set C++ standard for test
target_compile_features(test_layers PRIVATE cxx_std_23)

# Add compiler flags for tests
if(MSVC)
    target_compile_options(test_layers PRIVATE /W4)
else()
    target_compile_options(test_layers PRIVATE -Wall -Wextra)
endif()

# Register the test with CTest
add_test(NAME LayersLibraryTest COMMAND test_layers)

# Set test properties
set_tests_properties(LayersLibraryTest PROPERTIES
    TIMEOUT 30
    PASS_REGULAR_EXPRESSION "All tests passed!"
)