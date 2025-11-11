#!/bin/bash
# run.sh - Run script for DigitRecognition project

set -e  # Exit on any error

echo -e "\033[32mRunning DigitRecognition project...\033[0m"

# Check if build directory exists
if [ ! -d "build" ]; then
    echo -e "\033[31mBuild directory not found. Please run build.sh first.\033[0m"
    exit 1
fi

# Check if executable exists (try common Unix naming patterns)
exe_path=""
if [ -f "build/DigitRecognition" ]; then
    exe_path="build/DigitRecognition"
elif [ -f "build/digitrecognition" ]; then
    exe_path="build/digitrecognition"
elif [ -f "build/main" ]; then
    exe_path="build/main"
else
    echo -e "\033[31mExecutable not found. Please run build.sh first.\033[0m"
    exit 1
fi

echo -e "\033[36mStarting application...\033[0m"
echo -e "\033[33m=====================================\n\033[0m"

# Run the executable
if ./"$exe_path"; then
    echo -e "\n\033[33m=====================================\033[0m"
    echo -e "\033[32mApplication completed successfully!\033[0m"
else
    exit_code=$?
    echo -e "\n\033[33m=====================================\033[0m"
    echo -e "\033[31mApplication exited with error code: $exit_code\033[0m"
    exit $exit_code
fi

echo -e "\033[32mRun script completed.\033[0m"
