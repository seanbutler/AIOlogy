#include <iostream>
#include <iomanip>
#include "libs/layers/layers.h"
#include "libs/config/config.hpp"

int main() {
    std::cout << "Testing Weight Initialization Configuration\n";
    std::cout << std::string(50, '=') << "\n\n";

    // Test uniform initialization [-0.5, 0.5]
    std::cout << "1. Testing uniform initialization [-0.5, 0.5]:\n";
    ANN::WeightInitConfig uniform_config;
    uniform_config.method = "uniform";
    uniform_config.range = {-0.5, 0.5};
    
    ANN::Layer uniform_layer(4, 3, uniform_config);
    std::cout << "First 10 weights: ";
    for (int i = 0; i < std::min(10, (int)uniform_layer.weights_.size()); i++) {
        std::cout << std::fixed << std::setprecision(3) << uniform_layer.weights_[i] << " ";
    }
    std::cout << "\n\n";

    // Test normal initialization (mean=0, std=0.1)
    std::cout << "2. Testing normal initialization (mean=0, std=0.1):\n";
    ANN::WeightInitConfig normal_config;
    normal_config.method = "normal";
    normal_config.range = {0.0, 0.1};
    
    ANN::Layer normal_layer(4, 3, normal_config);
    std::cout << "First 10 weights: ";
    for (int i = 0; i < std::min(10, (int)normal_layer.weights_.size()); i++) {
        std::cout << std::fixed << std::setprecision(3) << normal_layer.weights_[i] << " ";
    }
    std::cout << "\n\n";

    // Test Xavier initialization
    std::cout << "3. Testing Xavier initialization:\n";
    ANN::WeightInitConfig xavier_config;
    xavier_config.method = "xavier";
    
    ANN::Layer xavier_layer(4, 3, xavier_config);
    std::cout << "First 10 weights: ";
    for (int i = 0; i < std::min(10, (int)xavier_layer.weights_.size()); i++) {
        std::cout << std::fixed << std::setprecision(3) << xavier_layer.weights_[i] << " ";
    }
    std::cout << "\n\n";

    // Test He initialization
    std::cout << "4. Testing He initialization:\n";
    ANN::WeightInitConfig he_config;
    he_config.method = "he";
    
    ANN::Layer he_layer(4, 3, he_config);
    std::cout << "First 10 weights: ";
    for (int i = 0; i < std::min(10, (int)he_layer.weights_.size()); i++) {
        std::cout << std::fixed << std::setprecision(3) << he_layer.weights_[i] << " ";
    }
    std::cout << "\n\n";

    // Test config file loading
    std::cout << "5. Testing config file loading:\n";
    ANN::Config config;
    if (config.load("config.json")) {
        std::cout << "Weight init method: " << config.network.weight_init.method << "\n";
        std::cout << "Weight init range: [" << config.network.weight_init.range[0] 
                  << ", " << config.network.weight_init.range[1] << "]\n";
        
        // Create layer with config
        ANN::WeightInitConfig config_weight;
        config_weight.method = config.network.weight_init.method;
        config_weight.range = config.network.weight_init.range;
        
        ANN::Layer config_layer(4, 3, config_weight);
        std::cout << "First 10 weights from config: ";
        for (int i = 0; i < std::min(10, (int)config_layer.weights_.size()); i++) {
            std::cout << std::fixed << std::setprecision(3) << config_layer.weights_[i] << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "Failed to load config.json\n";
    }

    return 0;
}