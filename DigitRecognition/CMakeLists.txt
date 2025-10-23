# CMakeLists.txt for DigitRecognition project
cmake_minimum_required(VERSION 3.16)

# Include FetchContent for downloading dependencies
include(FetchContent)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Project name and version
project(DigitRecognition VERSION 0.3.0 LANGUAGES CXX)

# Fetch SDL2 and SDL2_image dependencies (more stable than SDL3)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.8  # Use stable SDL2 release
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.8.2  # Use stable SDL2_image release
    GIT_SHALLOW TRUE
)

# Fetch nlohmann/json for configuration and data serialization
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3  # Use stable nlohmann/json release
    GIT_SHALLOW TRUE
)

# Make SDL2 and SDL2_image available
set(SDL2_DISABLE_INSTALL ON CACHE BOOL "Disable SDL2 installation")
set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "Build SDL2_image samples")
set(SDL2IMAGE_TESTS OFF CACHE BOOL "Build SDL2_image tests")

FetchContent_MakeAvailable(SDL2 SDL2_image nlohmann_json)

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
    # Enhanced debugging flags for Visual Studio
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /W4 /DEBUG:FULL /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG /MD")
    
else()
    # Enhanced debugging flags for GCC/Clang
    set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -Wall -Wextra -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

# Ensure we're building in Debug mode by default for development
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
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
add_subdirectory(libs/images)
add_subdirectory(libs/training)
add_subdirectory(libs/config)


# Link libraries (add any external libraries you need)
target_link_libraries(${PROJECT_NAME} PRIVATE
    layers
    activations
    images
    training
    config
    nlohmann_json::nlohmann_json
)

# Set debugging properties for Visual Studio
if(MSVC)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()

# Function to copy DLLs to target directory
function(copy_sdl_dlls target_name target_dir)
    if(WIN32)
        # Copy SDL2 DLL
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL2::SDL2>
                ${target_dir}
            COMMENT "Copying SDL2 DLL to ${target_dir}"
        )
        
        # Copy SDL2_image DLL
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL2_image>
                ${target_dir}
            COMMENT "Copying SDL2_image DLL to ${target_dir}"
        )
    endif()
endfunction()

# Copy SDL DLLs to main executable directory
copy_sdl_dlls(${PROJECT_NAME} $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# Installation rules (optional)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Install SDL DLLs alongside the executable
if(WIN32)
    install(FILES 
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE:SDL2_image>
        DESTINATION bin
    )
endif()