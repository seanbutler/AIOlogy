#include <iostream>
#include <cmath>
#include <vector>

namespace activations {

    // Sigmoid activation function
    double sigmoid(double x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    // ReLU activation function
    double relu(double x) {
        return std::max(0.0, x);
    }

} // namespace activations