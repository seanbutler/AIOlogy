# DigitRecognition

![Version](https://img.shields.io/badge/version-0.3.0-blue)
![Status](https://img.shields.io/badge/status-stable-green)
![C++](https://img.shields.io/badge/C++-23-blue)

A C++ neural network implementation for (in the first case) handwritten digit recognition using the MNIST dataset, but should be suitable for learning in the general case.

## Overview

This project implements a **from-scratch neural network** in C++ setup to be trained on the MNIST image data and then recognize handwritten digits (0-9). The implementation focuses on educational clarity while using c++ to afford some  performance, featuring:

- **Custom Neural Network Layers** - Fully connected layers with configurable activation functions
- **Activation Functions Library** - Sigmoid, ReLU implementations
- **JSON Configuration System** - Easy experimentation with network architectures and training parameters
- **MNIST Dataset Integration** - Automatic download and preprocessing of training data
- **Modular Architecture** - Clean separation between layers, activations, configuration, and network management
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
│   ├── activations/         # Activation functions (sigmoid, ReLU)
│   ├── config/              # JSON configuration management
│   ├── images/              # Image loading and preprocessing
│   ├── layers/              # Neural network layer implementation
│   ├── networks/            # Network management and training
│   └── training/            # Training dataset management
├── scripts/                 # Build and utility scripts
│   ├── build.ps1           # Build the entire project (Windows)
│   ├── build.sh            # Build the entire project (Linux/macOS)
│   ├── run.ps1             # Execute the main application (Windows)
│   ├── run.sh              # Execute the main application (Linux/macOS)
│   ├── test.ps1            # Run all unit tests (Windows)
│   ├── setup_env.sh        # Setup Python virtual environment (Linux/macOS)
│   ├── get_mnist.py        # Download MNIST dataset (Windows)
│   ├── get_mnist.sh        # Download MNIST dataset (Linux/macOS)
│   └── get_mnist_simple.py # Lightweight MNIST downloader
├── .gitignore              # Git ignore patterns
├── CHANGELOG.md            # Detailed version history
├── CMakeLists.txt          # Main CMake configuration with version
├── config.json             # Neural network configuration file
├── main.cpp                # Main application entry point
├── README.md               # Project overview and documentation
├── version.h               # Generated version header (auto-created)
└── version.h.in            # Version template for C++ code generation
```

## Platform Support

This project supports both Windows and Linux/macOS with platform-specific build scripts:

### Windows (PowerShell)
- Uses PowerShell scripts (`.ps1`)
- Manual Python environment setup
- Separate MNIST download step

### Linux/macOS (Bash)
- Uses Bash scripts (`.sh`)
- Automated Python virtual environment setup
- Integrated MNIST download during build
- Lightweight Python dependencies (no torch/torchvision)

## Quick Start

### Prerequisites

#### Windows
- **C++ Compiler** with C++23 support (Visual Studio 2022 recommended)
- **CMake** 3.16 or higher
- **Python** 3.7+ (for MNIST data download)
- **PowerShell** (for build scripts)

#### Linux/macOS
- **C++ Compiler** with C++23 support (GCC 11+ or Clang 14+)
- **CMake** 3.16 or higher
- **Python** 3.7+ (for MNIST data download)
- **Bash** shell (for build scripts)
- **Build essentials** (`sudo apt install build-essential` on Ubuntu/Debian)

### Dependencies

#### C++ Dependencies (All Platforms)
- **nlohmann/json** - JSON parsing for configuration (automatically downloaded via CMake)
- **SDL2** - Image loading support (automatically downloaded via CMake)

#### Python Dependencies (Linux/macOS)
- **pillow** ≥ 10.0.0 - Image processing for MNIST conversion
- **numpy** ≥ 1.24.0 - Numerical operations for image data

*Python dependencies are automatically installed in a virtual environment during the build process on Linux/macOS.*

### 1. Build the Project

#### Windows (PowerShell)
```powershell
.\scripts\build.ps1
```

#### Linux/macOS (Bash)
```bash
./scripts/build.sh
```

**What this does:**
- Creates the `build/` directory
- Sets up Python virtual environment (Linux/macOS only)
- Downloads MNIST dataset if not present
- Runs CMake configuration for your platform
- Compiles all libraries (activations, layers, networks)
- Builds the main application and test executables
- Links everything together

### 2. Download Training Data

#### Windows (PowerShell)
```powershell
python .\scripts\get_mnist.py
```

#### Linux/macOS (Bash)
```bash
./scripts/get_mnist.sh
```

**What this does:**
- Downloads the MNIST dataset (60,000 training + 10,000 test images)
- Converts images to PNG format organized by digit class
- Creates `mnist_images/train/0/`, `mnist_images/train/1/`, etc.
- Provides visual inspection of the training data

**Note:** On Linux/macOS, MNIST data is automatically downloaded during the build process if not present.

### 3. Run Tests

#### Windows (PowerShell)
```powershell
.\scripts\test.ps1
```

#### Linux/macOS (Bash)
```bash
cd build && ctest --verbose
```

**What this does:**
- Executes all unit tests using CTest
- Tests activation functions (sigmoid, ReLU)
- Validates neural network layer mathematics
- Verifies forward propagation calculations
- Shows detailed test results and coverage

### 4. Configure Your Network (Optional)

Edit `config.json` to customize your neural network:

```json
{
  "network": {
    "layers": [784, 64, 32, 10],
    "learning_rate": 0.001,
    "activation": "relu"
  },
  "training": {
    "epochs": 2,
    "shuffle": true
  }
}
```

**Configuration Options:**
- **layers**: Network architecture (input → hidden → output)
- **learning_rate**: How fast the network learns (0.001 - 0.01 typical)
- **epochs**: Number of training iterations
- **activation**: Activation function ("sigmoid" or "relu")

### 5. Run the Application

#### Windows (PowerShell)
```powershell
.\scripts\run.ps1
```

#### Linux/macOS (Bash)
```bash
./scripts/run.sh
```

**What this does:**
- Loads configuration from `config.json`
- Displays network architecture and training parameters
- Executes the main digit recognition program
- Demonstrates neural network functionality
- Shows sample predictions and accuracy metrics

## Configuration System

The project uses a JSON-based configuration system for easy experimentation:

### Network Configuration
```json
"network": {
  "layers": [784, 128, 64, 10],     // Network architecture
  "learning_rate": 0.01,            // Learning rate for training
  "activation": "sigmoid"           // Activation function
}
```

### Training Configuration
```json
"training": {
  "epochs": 5,                      // Number of training epochs
  "shuffle": true                   // Shuffle training data
}
```

### Data Configuration
```json
"data": {
  "train_path": "./data/mnist_images/train/",
  "test_path": "./data/mnist_images/test/",
  "image_size": [28, 28],
  "normalize": true
}
```

**Benefits:**
- **Easy Experimentation** - Try different architectures without recompiling
- **Reproducible Results** - Save exact configurations used for experiments
- **Parameter Validation** - Automatic validation of configuration values

## Complete Example

Here's how all the components work together:

```cpp
#include "libs/config/config.hpp"
#include "libs/networks/networks.hpp"
#include "libs/training/training.hpp"
#include "libs/images/images.hpp"

int main() {
    // 1. Load configuration
    ANN::Config config("config.json");
    config.print();  // Display loaded settings
    
    // 2. Create network from config
    ANN::Network network(config.network.layers, config.network.learning_rate);
    
    // 3. Load training data
    ANN::TrainingSet training_set;
    for (const auto& entry : std::filesystem::directory_iterator(config.data.train_path)) {
        if (entry.path().extension() == ".png") {
            std::string filename = entry.path().filename().string();
            int label = std::stoi(filename.substr(0, filename.find('_')));
            
            std::vector<double> image_data = ANN::load_image(entry.path().string());
            training_set.add_instance({image_data, label, filename});
        }
    }
    
    // 4. Train the network
    auto instances = training_set.get_instances();
    for (int epoch = 0; epoch < config.training.epochs; ++epoch) {
        for (const auto& instance : instances) {
            network.train(instance.input_data, instance.label);
        }
    }
    
    // 5. Test the network
    std::vector<double> test_image = ANN::load_image("test_image.png");
    int prediction = network.predict_label(test_image);
    std::cout << "Predicted digit: " << prediction << std::endl;
    
    return 0;
}
```

## Core Components

### Configuration Management (`libs/config/`)

JSON-based configuration system featuring:

- **Structured Configuration** - Organized sections for network, training, and data
- **Automatic Parsing** - Load settings from `config.json` with fallback defaults
- **Validation** - Verify configuration values are valid before training
- **Easy Experimentation** - Change network architecture without recompilation

```cpp
// Example usage:
ANN::Config config("config.json");
ANN::Network network(config.network.layers, config.network.learning_rate);
```

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

Implements:
- **Sigmoid** – Smooth activation for binary classification and output layers
- **ReLU** – Fast activation for hidden layers

Both are fully tested for correctness and edge cases.

### Image Processing (`libs/images/`)

MNIST image loading and preprocessing:

- **PNG Image Loading** - Read MNIST dataset converted to PNG format
- **Data Normalization** - Scale pixel values for optimal training
- **Format Conversion** - Convert image data to neural network input format

```cpp
// Example usage:
std::vector<double> image_data = ANN::load_image("data/mnist_images/train/5/5_12345.png");
// Returns normalized pixel values as vector of doubles (784 elements for 28x28 image)
```

### Training Management (`libs/training/`)

Dataset management and training utilities:

- **Training Set Organization** - Manage labeled training instances
- **Data Validation** - Ensure proper dataset structure
- **Batch Processing** - Efficient handling of training data

```cpp
// Example usage:
ANN::TrainingSet training_set;
training_set.add_instance({image_data, 5, "5_12345.png"});  // image, label, filename
auto instances = training_set.get_instances();
std::cout << "Training set size: " << instances.size() << std::endl;
```

### Network Management (`libs/networks/`)

High-level network construction and training:

- **Configuration-Driven Architecture** - Build networks from JSON configuration
- **Training Pipeline** - Forward/backward pass coordination with loss calculation
- **Layer Management** - Add, configure, and connect layers automatically
- **Prediction Interface** - Easy-to-use prediction methods for inference

```cpp
// Example usage:
ANN::Config config("config.json");
ANN::Network network(config.network.layers, config.network.learning_rate);

// Training
network.train(image_data, label);

// Prediction
int predicted_label = network.predict_label(image_data);
std::vector<double> probabilities = network.predict_probabilities(image_data);
```

## Educational Features

This project is designed for learning neural networks:

- No hidden complexity or black-box libraries
- Unit tests demonstrate expected behavior
- Test cases show mathematical correctness
- Easy to experiment with different inputs

- Works with actual MNIST handwritten digits
- Visualizable training data
- Measurable performance metrics

## Automated Setup (Linux/macOS)

The Linux/macOS build system provides fully automated setup:

```bash
# One command does everything:
./scripts/build.sh

# What happens automatically:
# 1. Creates Python virtual environment (./venv/)
# 2. Installs minimal dependencies (pillow, numpy)
# 3. Downloads MNIST dataset if not present
# 4. Configures CMake build system
# 5. Compiles all C++ components
# 6. Links final executable
```

**Benefits:**
- **Zero Configuration** - Works out of the box
- **Minimal Dependencies** - No heavy ML frameworks required
- **Fast Setup** - Lightweight Python environment
- **Reproducible** - Consistent environment across machines

## Development Workflow

#### Windows
1. **Configure** - Edit `config.json` to set network architecture and training parameters
2. **Build** - Run `.\scripts\build.ps1` to compile changes
3. **Test** - Run `.\scripts\test.ps1` to validate functionality
4. **Train** - Execute `.\scripts\run.ps1` to train and test the network
5. **Iterate** - Modify configuration and repeat for experimentation

#### Linux/macOS
1. **Configure** - Edit `config.json` to set network architecture and training parameters
2. **Build** - Run `./scripts/build.sh` to compile changes (includes environment setup)
3. **Test** - Run `cd build && ctest --verbose` to validate functionality
4. **Train** - Execute `./scripts/run.sh` to train and test the network
5. **Iterate** - Modify configuration and repeat for experimentation


## Technical Details


### Build System

- **CMake** for cross-platform building with automatic dependency management
- **FetchContent** integration for nlohmann/json and SDL2 libraries
- **Modular Libraries** for clean dependency management
- **C++23 Standard** for modern language features
- **MSVC/GCC Support** with appropriate compiler flags


### Performance Considerations (Further Improvements Possible!)

- **Efficient Memory Layout** - Flat vectors for weight storage
- **Smart Pointers** - Automatic memory management
- **Minimal Copying** - Move semantics where appropriate


### Testing Framework

- **CTest Integration** - Automated test discovery
- **Assertion-Based Testing** - Clear pass/fail criteria
- **Verbose Output** - Detailed information for debugging


## Future Enhancements

- **Loss Curve Plotting** - CSV output for training visualization
- **Validation Loss Tracking** – Add validation loss calculation and reporting for better generalization monitoring
- **Convolutional Layers** - Add CNN support for better image recognition
- **GPU Acceleration** - CUDA or OpenCL integration
- **Model Serialization** - Save/load trained networks to/from JSON
- **Performance Optimization** - SIMD instructions, multithreading
- **Additional Activation Functions** - Tanh, Leaky ReLU, Swish implementations


## Contributing

You are welcome to contribute. Just reach out. Meanwhile, lets...

1. Maintain code clarity over performance optimization
2. Include comprehensive comments explaining mathematical concepts
3. Add corresponding unit tests for new features
4. Update documentation for any API changes

---

Share & Enjoy!