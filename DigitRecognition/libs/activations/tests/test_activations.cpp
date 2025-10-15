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
    ASSERT_NEAR(activations::sigmoid(0.0), 0.5, 1e-10);
    ASSERT_NEAR(activations::sigmoid(-1000.0), 0.0, 1e-10);
    ASSERT_NEAR(activations::sigmoid(1000.0), 1.0, 1e-10);
    
    // Test symmetry: sigmoid(-x) + sigmoid(x) = 1
    double x = 2.5;
    ASSERT_NEAR(activations::sigmoid(-x) + activations::sigmoid(x), 1.0, 1e-10);
    
    std::cout << "✓ Sigmoid tests passed" << std::endl;
    return true;
}

bool test_relu() {
    std::cout << "Testing ReLU function..." << std::endl;
    
    // Test positive values
    ASSERT_NEAR(activations::relu(5.0), 5.0, 1e-10);
    ASSERT_NEAR(activations::relu(0.1), 0.1, 1e-10);
    
    // Test zero and negative values
    ASSERT_NEAR(activations::relu(0.0), 0.0, 1e-10);
    ASSERT_NEAR(activations::relu(-5.0), 0.0, 1e-10);
    ASSERT_NEAR(activations::relu(-0.1), 0.0, 1e-10);
    
    std::cout << "✓ ReLU tests passed" << std::endl;
    return true;
}

// bool test_softmax() {
//     std::cout << "Testing softmax function..." << std::endl;
    
//     // Test that outputs sum to 1
//     std::vector<double> input = {1.0, 2.0, 3.0, 4.0, 5.0};
//     auto result = activations::softmax(input);
    
//     double sum = 0.0;
//     for (double val : result) {
//         sum += val;
//         ASSERT_TRUE(val >= 0.0);  // All values should be non-negative
//     }
//     ASSERT_NEAR(sum, 1.0, 1e-10);  // Sum should be 1
    
//     // Test that larger inputs produce larger outputs
//     std::vector<double> simple_input = {1.0, 2.0, 3.0};
//     auto simple_result = activations::softmax(simple_input);
//     ASSERT_TRUE(simple_result[0] < simple_result[1]);
//     ASSERT_TRUE(simple_result[1] < simple_result[2]);
    
//     // Test single element
//     std::vector<double> single = {5.0};
//     auto single_result = activations::softmax(single);
//     ASSERT_NEAR(single_result[0], 1.0, 1e-10);
    
//     std::cout << "✓ Softmax tests passed" << std::endl;
//     return true;
// }

// bool test_derivatives() {
//     std::cout << "Testing derivative functions..." << std::endl;
    
//     // Test sigmoid derivative
//     double x = 1.0;
//     double sigmoid_val = activations::sigmoid(x);
//     double expected_derivative = sigmoid_val * (1.0 - sigmoid_val);
//     ASSERT_NEAR(activations::sigmoid_derivative(x), expected_derivative, 1e-10);
    
//     // Test ReLU derivative
//     ASSERT_NEAR(activations::relu_derivative(5.0), 1.0, 1e-10);
//     ASSERT_NEAR(activations::relu_derivative(-5.0), 0.0, 1e-10);
//     ASSERT_NEAR(activations::relu_derivative(0.0), 0.0, 1e-10);
    
//     std::cout << "✓ Derivative tests passed" << std::endl;
//     return true;
// }

bool test_factory_functions() {
    std::cout << "Testing factory functions..." << std::endl;
    
    // Test getting activation functions by name
    auto sigmoid_func = activations::get_activation("sigmoid");
    auto relu_func = activations::get_activation("relu");
    
    double test_val = 2.0;
    ASSERT_NEAR(sigmoid_func(test_val), activations::sigmoid(test_val), 1e-10);
    ASSERT_NEAR(relu_func(test_val), activations::relu(test_val), 1e-10);
    
    // Test activation pairs
    auto sigmoid_pair = activations::get_activation_pair("sigmoid");
    ASSERT_NEAR(sigmoid_pair.function(test_val), activations::sigmoid(test_val), 1e-10);
    ASSERT_NEAR(sigmoid_pair.derivative(test_val), activations::sigmoid_derivative(test_val), 1e-10);
    
    std::cout << "✓ Factory function tests passed" << std::endl;
    return true;
}

bool test_vector_operations() {
    std::cout << "Testing vector operations..." << std::endl;
    
    std::vector<double> input = {-2.0, -1.0, 0.0, 1.0, 2.0};
    auto sigmoid_func = activations::get_activation("sigmoid");
    auto result = activations::apply_activation(input, sigmoid_func);
    
    ASSERT_TRUE(result.size() == input.size());
    
    for (size_t i = 0; i < input.size(); ++i) {
        ASSERT_NEAR(result[i], activations::sigmoid(input[i]), 1e-10);
    }
    
    std::cout << "✓ Vector operation tests passed" << std::endl;
    return true;
}

int main() {
    std::cout << "Running Activations Library Tests" << std::endl;
    std::cout << "==================================" << std::endl;
    
    bool all_passed = true;
    
    all_passed &= test_sigmoid();
    all_passed &= test_relu();
    // all_passed &= test_softmax();
    // all_passed &= test_derivatives();
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