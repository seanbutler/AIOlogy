#include "../activations.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

// Simple test framework macros
#define ASSERT_NEAR(actual, expected, tolerance) \
    do { \
        if (std::abs((actual) - (expected)) > (tolerance)) { \
            std::cerr << "ASSERTION FAILED: " << #actual << " = " << (actual) \
                      << ", expected " << (expected) << " (tolerance " << (tolerance) << ")" << std::endl; \
            return false; \
        } \
    } while(0)

#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "ASSERTION FAILED: " << #condition << std::endl; \
            return false; \
        } \
    } while(0)

bool test_sigmoid() {
    std::cout << "Testing sigmoid function..." << std::endl;
    
    // Test known values
    ASSERT_NEAR(ANN::sigmoid(0.0), 0.5, 1e-10);
    ASSERT_NEAR(ANN::sigmoid(-1000.0), 0.0, 1e-10);
    ASSERT_NEAR(ANN::sigmoid(1000.0), 1.0, 1e-10);
    
    // Test symmetry: sigmoid(-x) + sigmoid(x) = 1
    double x = 2.5;
    ASSERT_NEAR(ANN::sigmoid(-x) + ANN::sigmoid(x), 1.0, 1e-10);
    
    std::cout << "✓ Sigmoid tests passed" << std::endl;
    return true;
}

bool test_relu() {
    std::cout << "Testing ReLU function..." << std::endl;

    // Test positive values
    ASSERT_NEAR(ANN::relu(5.0), 5.0, 1e-10);
    ASSERT_NEAR(ANN::relu(0.1), 0.1, 1e-10);

    // Test zero and negative values
    ASSERT_NEAR(ANN::relu(0.0), 0.0, 1e-10);
    ASSERT_NEAR(ANN::relu(-5.0), 0.0, 1e-10);
    ASSERT_NEAR(ANN::relu(-0.1), 0.0, 1e-10);

    std::cout << "✓ ReLU tests passed" << std::endl;
    return true;
}

bool test_sigmoid_derivative() {
    std::cout << "Testing sigmoid derivative..." << std::endl;
    // At x=0, derivative should be 0.25
    ASSERT_NEAR(ANN::sigmoid_derivative(0.0), 0.25, 1e-10);
    // At x=1000, derivative should be near 0
    ASSERT_NEAR(ANN::sigmoid_derivative(1000.0), 0.0, 1e-10);
    // At x=-1000, derivative should be near 0
    ASSERT_NEAR(ANN::sigmoid_derivative(-1000.0), 0.0, 1e-10);
    std::cout << "✓ Sigmoid derivative tests passed" << std::endl;
    return true;
}

bool test_relu_derivative() {
    std::cout << "Testing ReLU derivative..." << std::endl;
    ASSERT_NEAR(ANN::relu_derivative(5.0), 1.0, 1e-10);
    ASSERT_NEAR(ANN::relu_derivative(0.0), 0.0, 1e-10);
    ASSERT_NEAR(ANN::relu_derivative(-5.0), 0.0, 1e-10);
    std::cout << "✓ ReLU derivative tests passed" << std::endl;
    return true;
}

bool test_factory_functions() {
    std::cout << "Testing factory functions..." << std::endl;
    
    // Test getting activation functions by name
    auto sigmoid_func = ANN::get_activation("sigmoid");
    auto relu_func = ANN::get_activation("relu");
    
    double test_val = 2.0;
    ASSERT_NEAR(sigmoid_func(test_val), ANN::sigmoid(test_val), 1e-10);
    ASSERT_NEAR(relu_func(test_val), ANN::relu(test_val), 1e-10);
    
    // Test activation pairs
    // auto sigmoid_pair = ANN::get_activation_pair("sigmoid");
    // ASSERT_NEAR(sigmoid_pair.function(test_val), ANN::sigmoid(test_val), 1e-10);
    // ASSERT_NEAR(sigmoid_pair.derivative(test_val), ANN::sigmoid_derivative(test_val), 1e-10);
    
    std::cout << "✓ Factory function tests passed" << std::endl;
    return true;
}

bool test_vector_operations() {
    std::cout << "Testing vector operations..." << std::endl;
    
    std::vector<double> input = {-2.0, -1.0, 0.0, 1.0, 2.0};
    auto sigmoid_func = ANN::get_activation("sigmoid");
    auto result = ANN::apply_activation(input, sigmoid_func);
    
    ASSERT_TRUE(result.size() == input.size());
    
    for (size_t i = 0; i < input.size(); ++i) {
        ASSERT_NEAR(result[i], ANN::sigmoid(input[i]), 1e-10);
    }
    
    std::cout << "✓ Vector operation tests passed" << std::endl;
    return true;
}

bool test_edge_cases() {
    std::cout << "Testing edge cases..." << std::endl;
    double nan = std::numeric_limits<double>::quiet_NaN();
    double inf = std::numeric_limits<double>::infinity();
    double ninf = -std::numeric_limits<double>::infinity();
    // Sigmoid
    ASSERT_TRUE(std::isnan(ANN::sigmoid(nan)));
    ASSERT_NEAR(ANN::sigmoid(inf), 1.0, 1e-10);
    ASSERT_NEAR(ANN::sigmoid(ninf), 0.0, 1e-10);
    // ReLU
    ASSERT_TRUE(std::isnan(ANN::relu(nan)));
    ASSERT_NEAR(ANN::relu(inf), inf, 1e-10);
    ASSERT_NEAR(ANN::relu(ninf), 0.0, 1e-10);
    std::cout << "✓ Edge case tests passed" << std::endl;
    return true;
}

int main() {
    std::cout << "Running Activations Library Tests" << std::endl;
    std::cout << "==================================" << std::endl;
    bool all_passed = true;
    all_passed &= test_sigmoid();
    all_passed &= test_relu();
    all_passed &= test_sigmoid_derivative();
    all_passed &= test_relu_derivative();
    all_passed &= test_edge_cases();
    all_passed &= test_factory_functions();
    all_passed &= test_vector_operations();
    std::cout << std::endl;
    if (all_passed) {
        std::cout << "🎉 All tests passed!" << std::endl;
        return 0;
    } else {
        std::cout << "❌ Some tests failed!" << std::endl;
        return 1;
    }
}