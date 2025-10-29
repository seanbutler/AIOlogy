# CMakeLists.txt for layers tests
cmake_minimum_required(VERSION 3.16)

# Create test executable
add_executable(test_layers
    test_layers.cpp
)

# Create our new detailed layer test executable
add_executable(test_layer
    test_layer.cpp
)

# Add weight initialization test executable

add_executable(test_weight_init
    test_weight_init.cpp
)

# Link the weight init test with layers and config libraries
target_link_libraries(test_weight_init PRIVATE layers config)

# Set C++ standard for weight init test
target_compile_features(test_weight_init PRIVATE cxx_std_23)

# Add compiler flags for weight init test
if(MSVC)
    target_compile_options(test_weight_init PRIVATE /W4)
else()
    target_compile_options(test_weight_init PRIVATE -Wall -Wextra)
endif()

# Link the test executable with the layers library
target_link_libraries(test_layers PRIVATE layers)

# Link our new test executable
target_link_libraries(test_layer PRIVATE layers)

# Set C++ standard for test
target_compile_features(test_layers PRIVATE cxx_std_23)
target_compile_features(test_layer PRIVATE cxx_std_23)

# Add compiler flags for tests
if(MSVC)
    target_compile_options(test_layers PRIVATE /W4)
else()
    target_compile_options(test_layers PRIVATE -Wall -Wextra)
endif()

# Register the test with CTest
add_test(NAME LayersLibraryTest COMMAND test_layers)
add_test(NAME DetailedLayerTest COMMAND test_layer)

# Register weight initialization test with CTest
add_test(NAME WeightInitTest COMMAND test_weight_init)

# Set test properties
set_tests_properties(LayersLibraryTest PROPERTIES
    TIMEOUT 30
    PASS_REGULAR_EXPRESSION "All tests passed!"
)