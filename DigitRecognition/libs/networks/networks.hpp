#pragma once

#include <vector>
#include "../layers/layers.h"

namespace ANN {

    // Convert integer label to one-hot vector
    std::vector<double> label_to_one_hot_vector(int label, int num_classes = 10) {
        std::vector<double> one_hot(num_classes, 0.0);
        if (label >= 0 && label < num_classes) {
            one_hot[label] = 1.0;
        }
        return one_hot;
    }

    class Network {
        public:
            Network(const std::vector<int>&layer_sizes = {784, 128, 64, 10}, double lr = 0.01)
                : input_layer(layer_sizes[0], layer_sizes[1]),
                  output_layer(layer_sizes[layer_sizes.size()-2], layer_sizes[layer_sizes.size()-1]),
                  learning_rate(lr)
            {
                // Create hidden layers (if any)
                for(auto i = 1; i < layer_sizes.size() - 2; i++) {
                    layers.emplace_back(Layer(layer_sizes[i], layer_sizes[i+1]));
                }

                // Set up layer connectivity
                if (layers.empty()) {
                    // Direct connection: input -> output
                    input_layer.next_layer = std::make_shared<Layer>(output_layer);
                    output_layer.previous_layer = std::make_shared<Layer>(input_layer);
                } else {
                    // Chain: input -> hidden layers -> output
                    input_layer.next_layer = std::make_shared<Layer>(layers[0]);
                    layers[0].previous_layer = std::make_shared<Layer>(input_layer);
                    
                    for (int l = 0; l < layers.size() - 1; ++l) {
                        layers[l].next_layer = std::make_shared<Layer>(layers[l+1]);
                        layers[l+1].previous_layer = std::make_shared<Layer>(layers[l]);
                    }
                    
                    layers.back().next_layer = std::make_shared<Layer>(output_layer);
                    output_layer.previous_layer = std::make_shared<Layer>(layers.back());
                }
            };

            ~Network(){
                layers.clear();
            };

            double train(const std::vector<double>& input_data, const int label)
            {
                // Input validation
                if (input_data.size() != input_layer.inputs_.size()) {
                    throw std::runtime_error("Input size mismatch: expected " + 
                        std::to_string(input_layer.inputs_.size()) + ", got " + 
                        std::to_string(input_data.size()));
                }
                
                // Set input layer data
                input_layer.inputs_ = input_data;

                // Forward Pass - Chain layer outputs to next layer inputs
                input_layer.forward();
                
                // Pass input layer outputs to first hidden layer (if exists)
                if (!layers.empty()) {
                    layers[0].inputs_ = input_layer.outputs_;
                    
                    // Process hidden layers
                    for(size_t i = 0; i < layers.size(); ++i) {
                        layers[i].forward();
                        
                        // Pass current layer output to next layer input
                        if (i < layers.size() - 1) {
                            layers[i + 1].inputs_ = layers[i].outputs_;
                        }
                    }

                    // Pass last hidden layer output to output layer
                    output_layer.inputs_ = layers.back().outputs_;
                } else {
                    // Direct connection: input -> output (no hidden layers)
                    output_layer.inputs_ = input_layer.outputs_;
                }
                
                output_layer.forward();

                // Calculate loss
                double loss = 0.0;
                std::vector<double> target = label_to_one_hot_vector(label, static_cast<int>(output_layer.outputs_.size()));
                for (size_t i = 0; i < target.size(); ++i) {
                    double diff = output_layer.outputs_[i] - target[i];
                    loss += diff * diff; // Mean Squared Error
                }

                // Backward Pass - Calculate loss gradients for output layer
                std::vector<double> loss_gradients(output_layer.outputs_.size());
                for (size_t i = 0; i < output_layer.outputs_.size(); ++i) {
                    // MSE derivative: ∂Loss/∂output = 2 * (predicted - actual)
                    loss_gradients[i] = 2.0 * (output_layer.outputs_[i] - target[i]);
                }
                
                // Start backpropagation from output layer
                std::vector<double> gradients = output_layer.backward(output_layer.inputs_, loss_gradients);
                
                // Propagate backwards through hidden layers
                for (size_t i = layers.size(); i > 0; --i) {
                    gradients = layers[i-1].backward(layers[i-1].inputs_, gradients);
                }
                
                // Finally propagate to input layer (though input layer gradients aren't used)
                if (!layers.empty()) {
                    input_layer.backward(input_layer.inputs_, gradients);
                } else {
                    // Direct connection case
                    input_layer.backward(input_layer.inputs_, gradients);
                }

                // Update Weights and Biases for all layers
                
                // Update input layer
                for (size_t i = 0; i < input_layer.weights_.size(); ++i) {
                    input_layer.weights_[i] -= learning_rate * input_layer.weight_gradients_[i];
                }
                for (size_t i = 0; i < input_layer.biases_.size(); ++i) {
                    input_layer.biases_[i] -= learning_rate * input_layer.bias_gradients_[i];
                }
                
                // Update hidden layers
                for (auto& layer : layers) {
                    for (size_t i = 0; i < layer.weights_.size(); ++i) {
                        layer.weights_[i] -= learning_rate * layer.weight_gradients_[i];
                    }
                    for (size_t i = 0; i < layer.biases_.size(); ++i) {
                        layer.biases_[i] -= learning_rate * layer.bias_gradients_[i];
                    }
                }
                
                // Update output layer
                for (size_t i = 0; i < output_layer.weights_.size(); ++i) {
                    output_layer.weights_[i] -= learning_rate * output_layer.weight_gradients_[i];
                }
                for (size_t i = 0; i < output_layer.biases_.size(); ++i) {
                    output_layer.biases_[i] -= learning_rate * output_layer.bias_gradients_[i];
                }

                return loss;  // Return the calculated loss for this training sample
            }



            std::vector<double> predict_probabilities(const std::vector<double>& input_data) {
                // Input validation
                if (input_data.size() != input_layer.inputs_.size()) {
                    throw std::runtime_error("Input size mismatch: expected " + 
                        std::to_string(input_layer.inputs_.size()) + ", got " + 
                        std::to_string(input_data.size()));
                }
                
                // Set input layer data
                input_layer.inputs_ = input_data;

                // Forward Pass - Chain layer outputs to next layer inputs
                input_layer.forward();
                
                // Pass input layer outputs to first hidden layer (if exists)
                if (!layers.empty()) {
                    layers[0].inputs_ = input_layer.outputs_;
                    
                    // Process hidden layers - use size_t for loop counter
                    for(size_t i = 0; i < layers.size(); ++i) {
                        layers[i].forward();
                        
                        // Pass current layer output to next layer input
                        if (i < layers.size() - 1) {
                            layers[i + 1].inputs_ = layers[i].outputs_;
                        }
                    }
                    
                    // Pass last hidden layer output to output layer
                    output_layer.inputs_ = layers.back().outputs_;
                } else {
                    // Direct connection: input -> output (no hidden layers)
                    output_layer.inputs_ = input_layer.outputs_;
                }
                
                output_layer.forward();

                return output_layer.outputs_;
            }

            int predict_label(const std::vector<double>& input_data) {
                auto outputs = predict_probabilities(input_data);
                // Find index of max output 
                // TODO utility function?
                int predicted_label = 0;
                double max_value = outputs[0];
                for (size_t i = 1; i < outputs.size(); ++i) {
                    if (outputs[i] > max_value) {
                        max_value = outputs[i];
                        predicted_label = static_cast<int>(i);
                    }
                }
                return predicted_label;
            }
    private:
        Layer input_layer;
        std::vector<Layer> layers;
        Layer output_layer;
        double learning_rate;  // Learning rate for gradient descent

    };

} // namespace NN

