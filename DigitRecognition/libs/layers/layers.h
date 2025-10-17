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
            , activation_function(ANN::get_activation("sigmoid"))  // Default activation function
        {
            // Initialize weights randomly
            randomly_initialize_weights();
        }

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
            //
            // take all the activations from the previous layer or exit
            //
            if (previous_layer) {
                inputs_ = previous_layer->outputs_;
            }

            outputs_.assign(outputs_.size(), 0.0); // Reset outputs to zero before calculation

            //
            // calculate my outputs
            //

            for(auto output_index = 0; output_index< outputs_.size(); output_index++) {

                for(auto input_index = 0; input_index < inputs_.size(); input_index++) 
                {
                    auto input_times_weight = inputs_[input_index] * weights_[input_index + (output_index * inputs_.size())];
                    outputs_[output_index] += input_times_weight;   
                }
                
                outputs_[output_index] += biases_[output_index];
                outputs_[output_index] = activation_function(outputs_[output_index]);
            }

            return outputs_; // Just return input for now
        }

        // std::vector<double> backward(const std::vector<double>& input, const std::vector<double>& grad_output)
        // {
        //     // Backpropagation pass stub
        //     return input; // Just return input for now
        // }

        std::vector<double> inputs_;    // input values, place to store result of previous layer or set inputs if first layer
        std::vector<double> weights_;   // size = current neurons * previous neurons
        std::vector<double> biases_;    // size = current neurons. one bias per output neuron
        std::vector<double> outputs_;    // activation value, result of activation function

        std::unique_ptr<Layer> previous_layer; // pointer to previous layer, null if first layer
        std::unique_ptr<Layer> next_layer; // pointer to next layer, null if last layer

        std::function<double(double)> activation_function;      // activation function for this layer
    };


} // namespace layers