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
            Network(const std::vector<int>&layer_sizes = {784, 128, 64, 10})
                : input_layer(layer_sizes[0], layer_sizes[1]),
                  output_layer(layer_sizes[layer_sizes.size()-2], layer_sizes[layer_sizes.size()-1])
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

            void train(const std::vector<double>& input_data, const int label)
            {
                std::cout << "Training with input size: " << input_data.size() << " and label: " << label ;

                // Set input layer data
                input_layer.inputs_ = input_data;

                // Forward Pass
                input_layer.forward();
                for(auto& layer : layers) {
                    layer.forward();
                }
                output_layer.forward();

                // Calculate loss
                double loss = 0.0;
                std::vector<double> target = label_to_one_hot_vector(label, output_layer.outputs_.size());
                for (size_t i = 0; i < target.size(); ++i) {
                    double diff = output_layer.outputs_[i] - target[i];
                    loss += diff * diff; // Mean Squared Error
                }

                std::cout << " loss: " << loss << "\r";

                // Backward Pass
                output_layer.backward(output_layer.inputs_, {}); // grad_output is empty for now
                for (int i = layers.size() - 1; i >= 0; --i) {
                    layers[i].backward(layers[i].inputs_, {}); // grad_output is empty for now
                }
                input_layer.backward(input_layer.inputs_, {}); // grad_output is empty for now

                // // Update Weights and Biases (stub - not implemented)
                // for (auto& layer : layers) {
                //     // Placeholder for weight and bias updates
                //     layer.weights_ = layer.weights_ - (learning_rate * layer.gradients_);
                //     layer.biases_ = layer.biases_ - (learning_rate * layer.bias_gradients_);
                // }   
            }

    private:
        Layer input_layer;
        std::vector<Layer> layers;
        Layer output_layer;

    };

} // namespace NN

