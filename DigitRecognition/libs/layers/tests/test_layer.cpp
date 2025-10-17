#include "../layers.h"
#include "../../activations/activations.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>

namespace Tests {

// Helper function to compare floating point numbers
bool are_close(double a, double b, double tolerance = 1e-6) {
    return std::abs(a - b) < tolerance;
}

// Helper function to print vectors for debugging
void print_vector(const std::vector<double>& vec, const std::string& name) {
    std::cout << name << ": ";
    for (double val : vec) {
        std::cout << std::fixed << std::setprecision(4) << val << " ";
    }
    std::cout << std::endl;
}

class LayerTests {
public:
    static void test_layer_construction() {
        std::cout << "Testing Layer Construction..." << std::endl;
        
        // Test basic construction
        ANN::Layer layer(3, 2);  // 3 inputs, 2 outputs
        
        // Check sizes
        assert(layer.inputs_.size() == 3);
        assert(layer.outputs_.size() == 2);
        assert(layer.weights_.size() == 6);  // 3 * 2 = 6 weights
        
        // Check that weights are initialized (not all zero)
        bool has_non_zero = false;
        for (double w : layer.weights_) {
            if (w != 0.0) {
                has_non_zero = true;
                break;
            }
        }
        assert(has_non_zero);
        
        std::cout << "✓ Layer construction test passed" << std::endl;
    }
    
    static void test_simple_forward_pass() {
        std::cout << "Testing Simple Forward Pass..." << std::endl;
        
        ANN::Layer layer(2, 1);  // 2 inputs, 1 output
        
        // Set known weights for predictable results
        layer.weights_[0] = 0.5;  // input[0] -> output[0]
        layer.weights_[1] = 0.3;  // input[1] -> output[0]
        
        // Set inputs manually
        layer.inputs_[0] = 1.0;
        layer.inputs_[1] = 2.0;
        
        // Forward pass
        auto outputs = layer.forward();
        
        // Expected: 1.0 * 0.5 + 2.0 * 0.3 = 0.5 + 0.6 = 1.1
        // But now with sigmoid activation: sigmoid(1.1) ≈ 0.7503
        double raw_sum = (1.0 * 0.5) + (2.0 * 0.3);
        double expected = ANN::sigmoid(raw_sum);
        
        print_vector(layer.inputs_, "Inputs");
        print_vector(layer.weights_, "Weights");
        print_vector(outputs, "Outputs");
        
        std::cout << "Raw weighted sum: " << raw_sum << std::endl;
        std::cout << "After sigmoid: " << expected << std::endl;
        
        assert(are_close(outputs[0], expected));
        
        std::cout << "✓ Simple forward pass test passed" << std::endl;
    }
    
    static void test_multi_output_forward() {
        std::cout << "Testing Multi-Output Forward Pass..." << std::endl;
        
        ANN::Layer layer(2, 2);  // 2 inputs, 2 outputs
        
        // Set known weights
        // For output[0]: weights[0], weights[1]
        // For output[1]: weights[2], weights[3]
        layer.weights_[0] = 0.1;  // input[0] -> output[0]
        layer.weights_[1] = 0.2;  // input[1] -> output[0]
        layer.weights_[2] = 0.3;  // input[0] -> output[1]
        layer.weights_[3] = 0.4;  // input[1] -> output[1]
        
        // Set inputs
        layer.inputs_[0] = 1.0;
        layer.inputs_[1] = 2.0;
        
        auto outputs = layer.forward();
        
        // Expected calculations (raw weighted sums):
        // output[0] = 1.0 * 0.1 + 2.0 * 0.2 = 0.1 + 0.4 = 0.5
        // output[1] = 1.0 * 0.3 + 2.0 * 0.4 = 0.3 + 0.8 = 1.1
        // But now with sigmoid activation:
        double raw_sum0 = 0.5;
        double raw_sum1 = 1.1;
        double expected0 = ANN::sigmoid(raw_sum0);
        double expected1 = ANN::sigmoid(raw_sum1);
        
        print_vector(outputs, "Multi-outputs");
        
        assert(are_close(outputs[0], expected0));
        assert(are_close(outputs[1], expected1));
        
        std::cout << "✓ Multi-output forward pass test passed" << std::endl;
    }
    
    static void test_with_sigmoid_activation() {
        std::cout << "Testing Layer with Sigmoid Activation..." << std::endl;
        
        ANN::Layer layer(2, 1);
        
        // Set activation function to sigmoid
        layer.activation_function = ANN::sigmoid;
        
        // Set weights and inputs for known result
        layer.weights_[0] = 1.0;
        layer.weights_[1] = 1.0;
        layer.inputs_[0] = 0.0;  // Small inputs to avoid saturation
        layer.inputs_[1] = 0.0;
        
        auto outputs = layer.forward();
        
        // Note: Your current forward() doesn't apply activation function
        // This test will show that limitation
        
        print_vector(outputs, "Raw outputs (no activation applied)");
        
        // Apply sigmoid manually to show what it should be
        double with_sigmoid = ANN::sigmoid(outputs[0]);
        std::cout << "With sigmoid applied: " << with_sigmoid << std::endl;
        
        std::cout << "✓ Activation function test completed (shows current limitation)" << std::endl;
    }
    
    static void test_layer_chaining() {
        std::cout << "Testing Layer Chaining..." << std::endl;
        
        // Create two layers
        auto layer1 = std::make_unique<ANN::Layer>(2, 3);  // 2->3
        auto layer2 = std::make_unique<ANN::Layer>(3, 1);  // 3->1
        
        // Set up simple weights
        for (int i = 0; i < layer1->weights_.size(); ++i) {
            layer1->weights_[i] = 0.1;
        }
        for (int i = 0; i < layer2->weights_.size(); ++i) {
            layer2->weights_[i] = 0.2;
        }
        
        // Set inputs to first layer
        layer1->inputs_[0] = 1.0;
        layer1->inputs_[1] = 1.0;
        
        // Connect layers
        layer2->previous_layer = std::move(layer1);
        
        // Forward pass through second layer (should pull from first)
        auto final_outputs = layer2->forward();
        
        print_vector(final_outputs, "Chained layer outputs");
        
        std::cout << "✓ Layer chaining test completed" << std::endl;
    }
    
    static void run_all_tests() {
        std::cout << "=== Running ANN::Layer Tests ===" << std::endl << std::endl;
        
        test_layer_construction();
        std::cout << std::endl;
        
        test_simple_forward_pass();
        std::cout << std::endl;
        
        test_multi_output_forward();
        std::cout << std::endl;
        
        test_with_sigmoid_activation();
        std::cout << std::endl;
        
        test_layer_chaining();
        std::cout << std::endl;
        
        std::cout << "=== All Layer Tests Completed ===" << std::endl;
    }
};

} // namespace Tests

int main() {
    Tests::LayerTests::run_all_tests();
    return 0;
}