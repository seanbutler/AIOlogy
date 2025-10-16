# DigitRecognition

![Version](https://img.shields.io/badge/version-0.3.0-blue)
![Status](https://img.shields.io/badge/status-stable-green)
![C++](https://img.shields.io/badge/C++-23-blue)

A C++ neural network implementation for (in the first case) handwritten digit recognition using the MNIST dataset, but should be suitable for learning in the general case.

## Overview

This project implements a **from-scratch neural network** in C++ designed to recognize handwritten digits (0-9). The implementation focuses on educational clarity while maintaining performance, featuring:

- **Custom Neural Network Layers** - Fully connected layers with configurable activation functions
- **Activation Functions Library** - Sigmoid, ReLU, and Softmax implementations
- **MNIST Dataset Integration** - Automatic download and preprocessing of training data
- **Modular Architecture** - Clean separation between layers, activations, and network management
- **Comprehensive Testing** - Unit tests for all core components


## Motivations
 - Learning to understand the internals of ANNs
 - Learning to apply ANN to real data from external sources (we cant always pull our data from python functions)


## Project Structure

```
DigitRecognition/
├── .venv/                   # Python virtual environment
├── .vscode/                 # VS Code configuration
├── build/                   # CMake build output directory
├── data/                    # Data directory
│   ├── mnist_data/          # Raw MNIST dataset files (PyTorch format)
│   └── mnist_images/        # Processed MNIST images (PNG format)
├── libs/                    # Core neural network libraries
│   ├── activations/         # Activation functions (sigmoid, ReLU, softmax)
│   ├── layers/              # Neural network layer implementation
│   └── networks/            # Network management and training
├── scripts/                 # Build and utility scripts
│   ├── build.ps1           # Build the entire project
│   ├── run.ps1             # Execute the main application
│   ├── test.ps1            # Run all unit tests
│   └── get_mnist.py        # Download MNIST dataset
├── .gitignore              # Git ignore patterns
├── CHANGELOG.md            # Detailed version history
├── CMakeLists.txt          # Main CMake configuration with version
├── main.cpp                # Main application entry point
├── README.md               # Project overview and documentation
├── version.h               # Generated version header (auto-created)
└── version.h.in            # Version template for C++ code generation
```

## Quick Start

### Prerequisites

- **C++ Compiler** with C++23 support (Visual Studio 2022 recommended on Windows)
- **CMake** 3.16 or higher
- **Python** 3.7+ (for MNIST data download)
- **PowerShell** (for build scripts on Windows)

### 1. Build the Project

```powershell
.\scripts\build.ps1
```

**What this does:**
- Creates the `build/` directory
- Runs CMake configuration for your platform
- Compiles all libraries (activations, layers, networks)
- Builds the main application and test executables
- Links everything together

### 2. Download Training Data

```powershell
python .\scripts\get_mnist.py
```

**What this does:**
- Downloads the MNIST dataset (60,000 training + 10,000 test images)
- Converts images to PNG format organized by digit class
- Creates `mnist_images/train/0/`, `mnist_images/train/1/`, etc.
- Provides visual inspection of the training data

### 3. Run Tests

```powershell
.\scripts\test.ps1
```

**What this does:**
- Executes all unit tests using CTest
- Tests activation functions (sigmoid, ReLU, softmax)
- Validates neural network layer mathematics
- Verifies forward propagation calculations
- Shows detailed test results and coverage

### 4. Run the Application

```powershell
.\scripts\run.ps1
```

**What this does:**
- Executes the main digit recognition program
- Demonstrates neural network functionality
- Shows sample predictions and accuracy metrics

## Core Components

### Neural Network Layers (`libs/layers/`)

The heart of the implementation featuring:

- **Dense Layer Implementation** - Fully connected layers with weight matrices
- **Forward Propagation** - Mathematical computation: `output = activation(input × weights + bias)`
- **Activation Integration** - Seamless integration with activation functions
- **Layer Chaining** - Connect multiple layers to form deep networks

```cpp
// Example usage:
NN::Layer layer(784, 128);  // 784 inputs → 128 outputs
layer.activation_function = activations::sigmoid;
auto outputs = layer.forward();
```

### Activation Functions (`libs/activations/`)

Mathematical functions for non-linearity:

- **Sigmoid** - Smooth activation for binary classification
- **ReLU** - Fast activation for hidden layers  
- **Softmax** - Probability distribution for output layers
- **Derivatives** - For backpropagation (future implementation)

### Network Management (`libs/networks/`)

High-level network construction and training:

- **Network Builder** - Easy network architecture definition
- **Layer Management** - Add, configure, and connect layers
- **Training Pipeline** - Forward/backward pass coordination

## Educational Features

This project is designed for learning neural networks:

### 1. **Transparent Implementation**
- No hidden complexity or black-box libraries
- Clear mathematical operations at each step
- Extensive comments explaining the "why" behind code

### 2. **Comprehensive Testing**
- Unit tests demonstrate expected behavior
- Test cases show mathematical correctness
- Easy to experiment with different inputs

### 3. **Modular Design**
- Each component can be studied independently
- Clean interfaces between modules
- Easy to extend with new activation functions or layer types

### 4. **Real Dataset**
- Works with actual MNIST handwritten digits
- Visualizable training data
- Measurable performance metrics

## Development Workflow

### Making Changes

1. **Modify Code** - Edit source files in `libs/`
2. **Build** - Run `.\scripts\build.ps1` to compile changes
3. **Test** - Run `.\scripts\test.ps1` to validate functionality
4. **Run** - Execute `.\scripts\run.ps1` to see results

### Adding New Features

- **New Activation Function** - Add to `libs/activations/`
- **New Layer Type** - Extend `libs/layers/`
- **Network Improvements** - Modify `libs/networks/`

### Debugging

- **Build Errors** - Check CMake configuration and C++23 support
- **Test Failures** - Review mathematical implementations
- **Runtime Issues** - Verify MNIST data download completed

## Technical Details

### Build System

- **CMake** for cross-platform building
- **Modular Libraries** for clean dependency management
- **C++23 Standard** for modern language features
- **MSVC/GCC Support** with appropriate compiler flags

### Performance Considerations

- **Efficient Memory Layout** - Flat vectors for weight storage
- **Smart Pointers** - Automatic memory management
- **Minimal Copying** - Move semantics where appropriate

### Testing Framework

- **CTest Integration** - Automated test discovery
- **Assertion-Based Testing** - Clear pass/fail criteria
- **Verbose Output** - Detailed information for debugging

## Future Enhancements

Potential areas for expansion:

- **Backpropagation** - Implement learning algorithms
- **Convolutional Layers** - Add CNN support for better image recognition
- **GPU Acceleration** - CUDA or OpenCL integration
- **Model Serialization** - Save/load trained networks
- **Performance Optimization** - SIMD instructions, multithreading

## Contributing

This is an educational project focused on understanding neural networks from first principles. When contributing:

1. Maintain code clarity over performance optimization
2. Include comprehensive comments explaining mathematical concepts
3. Add corresponding unit tests for new features
4. Update documentation for any API changes

---

**Happy Learning!** 🧠✨

This implementation provides a solid foundation for understanding how neural networks work at the mathematical and implementation level.