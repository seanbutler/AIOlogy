# CMakeLists.txt for images library tests
cmake_minimum_required(VERSION 3.16)

# Test executable name
set(TEST_NAME test_images)

# Create the test executable
add_executable(${TEST_NAME}
    test_images.cpp
)

# Set C++ standard for the test
target_compile_features(${TEST_NAME} PRIVATE cxx_std_23)

# Link with the images library
target_link_libraries(${TEST_NAME} PRIVATE images)

# Include directories for the test
target_include_directories(${TEST_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Compiler-specific flags for the test
if(MSVC)
    target_compile_options(${TEST_NAME} PRIVATE /W4)
else()
    target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra)
endif()

# Set test properties
set_target_properties(${TEST_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

# Add the test to CTest
add_test(NAME ImagesLibraryTest COMMAND ${TEST_NAME})

# Set test timeout (in seconds)
set_tests_properties(ImagesLibraryTest PROPERTIES TIMEOUT 30)

# Set working directory for the test (so it can find MNIST data)
set_tests_properties(ImagesLibraryTest PROPERTIES 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Copy SDL DLLs to test executable directory (Windows only)
if(WIN32 AND TARGET SDL2::SDL2 AND TARGET SDL2_image)
    add_custom_command(TARGET ${TEST_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE_DIR:${TEST_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2_image>
            $<TARGET_FILE_DIR:${TEST_NAME}>
        COMMENT "Copying SDL DLLs to test directory"
    )
endif()