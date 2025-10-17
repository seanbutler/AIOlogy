# CMakeLists.txt for images library
cmake_minimum_required(VERSION 3.16)

# Library name
set(LIBRARY_NAME images)

# Add the library as STATIC
add_library(${LIBRARY_NAME} STATIC
    images.cpp
    images.hpp
    # Add more source files here as needed
)

# Set C++ standard for this library
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_23)

# Find and link SDL2_image
find_package(SDL2_image QUIET)
if(TARGET SDL2_image::SDL2_image)
    target_link_libraries(${LIBRARY_NAME} PRIVATE SDL2_image::SDL2_image SDL2::SDL2)
    target_compile_definitions(${LIBRARY_NAME} PRIVATE HAS_SDL_IMAGE=1)
    message(STATUS "Found SDL2_image package")
else()
    # Fallback to direct linking if package not found
    if(TARGET SDL2_image)
        target_link_libraries(${LIBRARY_NAME} PRIVATE SDL2_image SDL2::SDL2)
        target_compile_definitions(${LIBRARY_NAME} PRIVATE HAS_SDL_IMAGE=1)
        # Add include directories for SDL2
        target_include_directories(${LIBRARY_NAME} PRIVATE 
            ${CMAKE_BINARY_DIR}/_deps/sdl2-src/include
            ${CMAKE_BINARY_DIR}/_deps/sdl2_image-src/include
        )
        message(STATUS "Using SDL2_image target directly")
    else()
        message(WARNING "SDL2_image not found - image loading will be limited")
        target_compile_definitions(${LIBRARY_NAME} PRIVATE HAS_SDL_IMAGE=0)
    endif()
endif()

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

# Add tests subdirectory if testing is enabled
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
