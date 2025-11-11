#!/bin/bash
# setup_env.sh - Setup Python virtual environment for DigitRecognition project

set -e  # Exit on any error

echo -e "\033[32mSetting up Python environment for DigitRecognition project...\033[0m"

# Virtual environment directory
VENV_DIR="venv"

# Create virtual environment if it doesn't exist
if [ ! -d "$VENV_DIR" ]; then
    echo -e "\033[33mCreating Python virtual environment...\033[0m"
    python3 -m venv "$VENV_DIR"
    echo -e "\033[32mVirtual environment created in $VENV_DIR/\033[0m"
else
    echo -e "\033[33mVirtual environment already exists in $VENV_DIR/\033[0m"
fi

# Activate virtual environment
echo -e "\033[33mActivating virtual environment...\033[0m"
source "$VENV_DIR/bin/activate"

# Upgrade pip
echo -e "\033[33mUpgrading pip...\033[0m"
pip install --upgrade pip

# Install requirements
if [ -f "requirements.txt" ]; then
    echo -e "\033[33mInstalling Python dependencies...\033[0m"
    pip install -r requirements.txt
    echo -e "\033[32mPython dependencies installed successfully!\033[0m"
else
    echo -e "\033[31mNo requirements.txt found!\033[0m"
    exit 1
fi

echo -e "\033[32mPython environment setup completed!\033[0m"
echo -e "\033[36mTo activate the environment manually, run:\033[0m"
echo -e "\033[36m  source venv/bin/activate\033[0m"
echo -e "\033[36mTo deactivate, run:\033[0m"
echo -e "\033[36m  deactivate\033[0m"
