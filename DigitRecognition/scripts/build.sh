#!/bin/bash
# build.sh - Build script for DigitRecognition project

set -e  # Exit on any error

echo -e "\033[32mBuilding DigitRecognition project...\033[0m"

# Setup Python environment if it doesn't exist
if [ ! -d "venv" ]; then
    echo -e "\033[33mPython virtual environment not found, setting up...\033[0m"
    ./scripts/setup_env.sh
fi

# Download MNIST dataset if it doesn't exist
if [ ! -d "data/mnist_images/train" ] || [ ! -d "data/mnist_images/test" ]; then
    echo -e "\033[33mMNIST dataset not found, downloading...\033[0m"
    ./scripts/get_mnist.sh
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo -e "\033[33mCreating build directory...\033[0m"
    mkdir -p build
fi

# Change to build directory
cd build

# Generate build files
echo -e "\033[36mGenerating build files...\033[0m"
if cmake ..; then
    # Build the project
    echo -e "\033[36mBuilding project...\033[0m"
    if cmake --build .; then
        echo -e "\033[32mBuild completed successfully!\033[0m"
    else
        echo -e "\033[31mBuild failed!\033[0m"
        cd ..
        exit 1
    fi
else
    echo -e "\033[31mCMake configuration failed!\033[0m"
    cd ..
    exit 1
fi

# Return to project root
cd ..

echo -e "\033[32mBuild script completed.\033[0m"
