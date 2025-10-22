#pragma once

#include "../activations/activations.h"

#include <iostream>
#include <functional>
#include <memory>

#include <vector>
#include <string>
#include <random>
#include <iomanip>

namespace ANN {

    class Layer {
    public:
        Layer(const int input_size, const int output_size)
            : inputs_(input_size, 0.0)
            , outputs_(output_size, 0.0)
            , weights_(input_size * output_size, 0.0) // flat vector to hold weights, think of me as a 2d array, this is where the learning is recorded    
            , biases_(output_size, 0.0)
            , pre_activations_(output_size, 0.0)  // Initialize pre-activation storage
            , weight_gradients_(input_size * output_size, 0.0)  // Initialize gradient storage
            , bias_gradients_(output_size, 0.0)
            , activation_function(ANN::get_activation("sigmoid"))  // Default activation function
            , activation_derivative(ANN::sigmoid_derivative)   // Default activation derivative
        {
            // Initialize weights randomly
            randomly_initialize_weights();
        }

        ~Layer() = default;
        

        void randomly_initialize_weights()
        {
            std::mt19937 rng(std::random_device{}());
            std::uniform_real_distribution<double> dist(-1.0, 1.0); // random weights between -1 and 1
            for(auto& w : weights_) {
                w = dist(rng);
            }
        }

        std::vector<double> forward()
        {
            // Reset both pre-activations and outputs
            pre_activations_.assign(pre_activations_.size(), 0.0);
            outputs_.assign(outputs_.size(), 0.0);

            //
            // calculate my outputs
            //
            
            for(auto output_index = 0; output_index< outputs_.size(); output_index++) {

                for(auto input_index = 0; input_index < inputs_.size(); input_index++) 
                {
                    auto input_times_weight = inputs_[input_index] * weights_[input_index + (output_index * inputs_.size())];
                    pre_activations_[output_index] += input_times_weight;   
                }
                
                pre_activations_[output_index] += biases_[output_index];
                outputs_[output_index] = activation_function(pre_activations_[output_index]);
            }

            return outputs_;
        }

        std::vector<double> backward(const std::vector<double>& input, 
                                        const std::vector<double>& loss_gradients)
        {
            // A. Compute activation function derivatives using the stored pre-activation values
            std::vector<double> activation_gradients(outputs_.size());
            for (size_t i = 0; i < outputs_.size(); ++i) {
                activation_gradients[i] = activation_derivative(pre_activations_[i]);
            }
            
            // B. Compute error terms (δ = ∂Loss/∂z = ∂Loss/∂output × ∂output/∂z)
            std::vector<double> deltas(outputs_.size());
            for (size_t i = 0; i < outputs_.size(); ++i) {
                deltas[i] = loss_gradients[i] * activation_gradients[i];
            }
            
            // C. Compute weight gradients (∂Loss/∂weight = input × delta)
            for (size_t output_idx = 0; output_idx < outputs_.size(); ++output_idx) {
                for (size_t input_idx = 0; input_idx < inputs_.size(); ++input_idx) {
                    size_t weight_idx = input_idx + (output_idx * inputs_.size());
                    weight_gradients_[weight_idx] = inputs_[input_idx] * deltas[output_idx];
                }
            }
            
            // D. Compute bias gradients (∂Loss/∂bias = delta)
            for (size_t i = 0; i < biases_.size(); ++i) {
                bias_gradients_[i] = deltas[i];
            }
            
            // E. Compute input (weights * deltas) gradients to pass back to previous layer
            std::vector<double> input_gradients(inputs_.size(), 0.0);
            for (size_t input_idx = 0; input_idx < inputs_.size(); ++input_idx) {
                for (size_t output_idx = 0; output_idx < outputs_.size(); ++output_idx) {
                    size_t weight_idx = input_idx + (output_idx * inputs_.size());
                    input_gradients[input_idx] += weights_[weight_idx] * deltas[output_idx];
                }
            }
            
            return input_gradients;
        }

        std::vector<double> inputs_;    // input values, place to store result of previous layer or set inputs if first layer
        std::vector<double> weights_;   // size = current neurons * previous neurons
        std::vector<double> biases_;    // size = current neurons. one bias per output neuron
        std::vector<double> pre_activations_;  // pre-activation values (z = weights*inputs + bias)
        std::vector<double> outputs_;   // activation value, result of activation function
        
        // Gradient storage for backpropagation
        std::vector<double> weight_gradients_;  // ∂Loss/∂weights, same size as weights_
        std::vector<double> bias_gradients_;    // ∂Loss/∂biases, same size as biases_

        std::shared_ptr<Layer> previous_layer;  // pointer to previous layer, null if first layer
        std::shared_ptr<Layer> next_layer;      // pointer to next layer, null if last layer

        std::function<double(double)> activation_function;      // activation function for this layer
        std::function<double(double)> activation_derivative;   // derivative of activation function
    };

} // namespace layers