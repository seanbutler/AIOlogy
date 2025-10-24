#define _USE_MATH_DEFINES  // For M_PI on Windows
#include "activations.h"
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace ANN {

// Single-value activation functions
double sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

double relu(double x) {
    return std::max(0.0, x);
}


// Derivative functions
double sigmoid_derivative(double x) {
    double s = sigmoid(x);
    return s * (1.0 - s);
}

double relu_derivative(double x) {
    return x > 0 ? 1.0 : 0.0;
}

// Factory function to get activation by name
ActivationFunction get_activation(const std::string& name) {
    static std::unordered_map<std::string, ActivationFunction> activations_map = {
        {"sigmoid", sigmoid},
        {"relu", relu}
    };
    
    auto it = activations_map.find(name);
    if (it != activations_map.end()) {
        return it->second;
    }
    
    throw std::invalid_argument("Unknown activation function: " + name);
}


// Apply activation function to entire vector
std::vector<double> apply_activation(const std::vector<double>& input, 
                                   const ActivationFunction& func) {
    std::vector<double> result(input.size());
    std::transform(input.begin(), input.end(), result.begin(), func);
    return result;
}

// Test function
void test_activations() {
    std::cout << "=== Activations Library Test ===" << std::endl;
    
    double test_value = 2.0;
    std::cout << "Test value: " << test_value << std::endl;
    
    // Test individual functions
    std::cout << "Sigmoid: " << sigmoid(test_value) << std::endl;
    std::cout << "ReLU: " << relu(test_value) << std::endl;
    
    // Test factory pattern
    std::cout << "\n--- Factory Pattern Test ---" << std::endl;
    try {
        auto sigmoid_func = get_activation("sigmoid");
        std::cout << "Factory sigmoid: " << sigmoid_func(test_value) << std::endl;
        
        auto relu_func = get_activation("relu");
        std::cout << "Factory ReLU: " << relu_func(test_value) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    // Test vector application
    std::cout << "\n--- Vector Application Test ---" << std::endl;
    std::vector<double> test_vector = {-2.0, -1.0, 0.0, 1.0, 2.0};
    auto sigmoid_func = get_activation("sigmoid");
    auto result = apply_activation(test_vector, sigmoid_func);
    
    std::cout << "Input:  [";
    for (size_t i = 0; i < test_vector.size(); ++i) {
        std::cout << test_vector[i];
        if (i < test_vector.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    
    std::cout << "Sigmoid: [";
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i];
        if (i < result.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    


}

} // namespace activations