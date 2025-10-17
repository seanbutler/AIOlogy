#pragma once

#define _USE_MATH_DEFINES  // For M_PI on Windows
#include <vector>
#include <functional>
#include <cmath>
#include <algorithm>
#include <string>

namespace ANN {

// Function type aliases for better readability
using ActivationFunction = std::function<double(double)>;
using VectorActivationFunction = std::function<std::vector<double>(const std::vector<double>&)>;

// Single-value activation functions
double sigmoid(double x);
double relu(double x);

// Vector activation functions
std::vector<double> softmax(const std::vector<double>& input);

// Derivative functions (useful for backpropagation)
double sigmoid_derivative(double x);
double relu_derivative(double x);

// Factory function to get activation by name
ActivationFunction get_activation(const std::string& name);

// Convenience class for activation function pairs (function + derivative)
struct ActivationPair {
    ActivationFunction function;
    ActivationFunction derivative;
    std::string name;
    
    ActivationPair(ActivationFunction f, ActivationFunction d, const std::string& n)
        : function(f), derivative(d), name(n) {}
};

// Factory for activation pairs
ActivationPair get_activation_pair(const std::string& name);

// Apply activation function to entire vector
std::vector<double> apply_activation(const std::vector<double>& input, 
                                   const ActivationFunction& func);

// Test function
void test_activations();

} // namespace activations
