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
│   ├── activations/         # Activation functions (sigmoid, ReLU, softmax)
│   ├── config/              # JSON configuration management
│   ├── images/              # Image loading and preprocessing
│   ├── layers/              # Neural network layer implementation
│   ├── networks/            # Network management and training
│   └── training/            # Training dataset management
├── scripts/                 # Build and utility scripts
│   ├── build.ps1           # Build the entire project
│   ├── run.ps1             # Execute the main application
│   ├── test.ps1            # Run all unit tests
│   └── get_mnist.py        # Download MNIST dataset
├── .gitignore              # Git ignore patterns
├── CHANGELOG.md            # Detailed version history
├── CMakeLists.txt          # Main CMake configuration with version
├── config.json             # Neural network configuration file
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

### Dependencies

- **nlohmann/json** - JSON parsing for configuration (automatically downloaded via CMake)
- **SDL2** - Image loading support (automatically downloaded via CMake)

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

```powershell
.\scripts\run.ps1
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

Mathematical functions for non-linearity:

- **Sigmoid** - Smooth activation for binary classification
- **ReLU** - Fast activation for hidden layers  


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

## Development Workflow

1. **Configure** - Edit `config.json` to set network architecture and training parameters
2. **Build** - Run `.\scripts\build.ps1` to compile changes
3. **Test** - Run `.\scripts\test.ps1` to validate functionality
4. **Train** - Execute `.\scripts\run.ps1` to train and test the network
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