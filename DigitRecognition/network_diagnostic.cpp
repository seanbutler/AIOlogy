#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>
#include "libs/networks/networks.hpp"
#include "libs/config/config.hpp"
#include "libs/training/training.hpp"

void analyze_network_health(ANN::Network& network, const std::vector<ANN::TrainingInstance>& test_samples) {
    std::cout << "\n=== NETWORK HEALTH ANALYSIS ===\n";
    
    // Test on a few samples to check activations
    int samples_to_test = std::min(100, (int)test_samples.size());
    
    std::vector<double> weight_stats;
    std::vector<double> activation_stats;
    
    for (int i = 0; i < samples_to_test; i++) {
        const auto& sample = test_samples[i];
        
        // Set inputs and do forward pass
        network.input_layer.inputs_ = sample.input_data;
        network.input_layer.forward();
        
        // Check for dead neurons in input layer
        int dead_neurons = 0;
        for (double output : network.input_layer.outputs_) {
            if (output <= 0.0) dead_neurons++;
        }
        
        // Check hidden layers
        for (auto& layer : network.layers) {
            layer.forward();
            
            for (double output : layer.outputs_) {
                if (output <= 0.0) dead_neurons++;
                activation_stats.push_back(output);
            }
        }
        
        // Check output layer
        network.output_layer.forward();
        for (double output : network.output_layer.outputs_) {
            activation_stats.push_back(output);
        }
    }
    
    // Analyze weight distributions
    std::cout << "Weight Analysis:\n";
    for (const auto& weight : network.input_layer.weights_) {
        weight_stats.push_back(weight);
    }
    for (const auto& layer : network.layers) {
        for (const auto& weight : layer.weights_) {
            weight_stats.push_back(weight);
        }
    }
    for (const auto& weight : network.output_layer.weights_) {
        weight_stats.push_back(weight);
    }
    
    if (!weight_stats.empty()) {
        std::sort(weight_stats.begin(), weight_stats.end());
        double weight_min = weight_stats.front();
        double weight_max = weight_stats.back();
        double weight_median = weight_stats[weight_stats.size() / 2];
        
        double weight_sum = 0.0;
        for (double w : weight_stats) weight_sum += w;
        double weight_mean = weight_sum / weight_stats.size();
        
        double weight_var = 0.0;
        for (double w : weight_stats) {
            weight_var += (w - weight_mean) * (w - weight_mean);
        }
        weight_var /= weight_stats.size();
        double weight_std = std::sqrt(weight_var);
        
        std::cout << "  Total weights: " << weight_stats.size() << "\n";
        std::cout << "  Weight range: [" << std::fixed << std::setprecision(4) << weight_min << ", " << weight_max << "]\n";
        std::cout << "  Weight mean: " << weight_mean << ", std: " << weight_std << "\n";
        std::cout << "  Weight median: " << weight_median << "\n";
    }
    
    // Analyze activations
    if (!activation_stats.empty()) {
        std::sort(activation_stats.begin(), activation_stats.end());
        double act_min = activation_stats.front();
        double act_max = activation_stats.back();
        double act_median = activation_stats[activation_stats.size() / 2];
        
        int zero_activations = 0;
        for (double a : activation_stats) {
            if (a == 0.0) zero_activations++;
        }
        
        std::cout << "\nActivation Analysis:\n";
        std::cout << "  Total activations tested: " << activation_stats.size() << "\n";
        std::cout << "  Activation range: [" << std::fixed << std::setprecision(4) << act_min << ", " << act_max << "]\n";
        std::cout << "  Zero activations: " << zero_activations << " (" 
                  << std::fixed << std::setprecision(1) << (100.0 * zero_activations / activation_stats.size()) << "%)\n";
        std::cout << "  Activation median: " << act_median << "\n";
        
        if (zero_activations > activation_stats.size() * 0.5) {
            std::cout << "  ⚠️  WARNING: More than 50% dead neurons detected!\n";
        }
    }
    
    std::cout << "=================================\n\n";
}

int main() {
    std::cout << "Network Health Diagnostic Tool\n";
    std::cout << std::string(40, '=') << "\n\n";
    
    // Load configuration
    Config config;
    if (!config.load("config.json")) {
        std::cerr << "Failed to load config.json\n";
        return 1;
    }
    
    config.print();
    
    // Create network with weight configuration
    ANN::WeightInitConfig weight_config;
    weight_config.method = config.network.weight_init.method;
    weight_config.range = config.network.weight_init.range;
    
    ANN::Network network(config.network.layers, config.network.learning_rate, weight_config);
    
    // Load some test data
    ANN::TrainingSet training_set;
    
    std::cout << "\nLoading test data for analysis...\n";
    
    int samples_loaded = 0;
    for (const auto& entry : std::filesystem::directory_iterator(config.data.test_path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".png" && samples_loaded < 200) {
            std::string filename = entry.path().filename().string();
            int label = std::stoi(filename.substr(0, filename.find('_')));
            std::vector<double> image_data = ANN::load_image(entry.path().string());
            
            training_set.add_instance(image_data, label);
            samples_loaded++;
        }
    }
    
    std::cout << "Loaded " << samples_loaded << " test samples for analysis.\n";
    
    if (samples_loaded > 0) {
        analyze_network_health(network, training_set.get_instances());
    }
    
    return 0;
}