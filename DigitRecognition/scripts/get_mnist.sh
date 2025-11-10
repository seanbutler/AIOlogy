#!/bin/bash
# get_mnist.sh - Download MNIST dataset using Python virtual environment

set -e  # Exit on any error

echo -e "\033[32mDownloading MNIST dataset...\033[0m"

# Check if virtual environment exists
if [ ! -d "venv" ]; then
    echo -e "\033[31mPython virtual environment not found!\033[0m"
    echo -e "\033[33mPlease run: ./scripts/setup_env.sh\033[0m"
    exit 1
fi

# Activate virtual environment
echo -e "\033[33mActivating virtual environment...\033[0m"
source venv/bin/activate

# Check if MNIST data already exists
if [ -d "data/mnist_images/train" ] && [ -d "data/mnist_images/test" ]; then
    echo -e "\033[33mMNIST dataset already exists, skipping download...\033[0m"
    echo -e "\033[36mTo force re-download, delete the data/mnist_images directory\033[0m"
    deactivate
    exit 0
fi

# Run the MNIST download script
echo -e "\033[33mRunning MNIST download script...\033[0m"
python3 scripts/get_mnist_simple.py

echo -e "\033[32mMNIST dataset downloaded successfully!\033[0m"

# Deactivate virtual environment
deactivate
