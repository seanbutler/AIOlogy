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
            {
                for(auto i = 0; i < layer_sizes.size() - 1; i++) {
                    layers.emplace_back(Layer(layer_sizes[i], layer_sizes[i+1]));
                }
            };

            ~Network(){
                layers.clear();
            };

            void train(const std::vector<double>& input_data, const std::vector<double>& result_data)
            {
                std::cout << "Training with input size: " << input_data.size() << " and result size: " << result_data.size() << std::endl;

                // Set input layer data
                layers[0].inputs_ = input_data;

                // Forward pass through all layers
                for(auto& layer : layers) {
                    layer.forward();
                }
            }

            void train(const std::vector<double>& input_data, const int label)
            {
                std::cout << "Training on label: " << label << " with input size: " << input_data.size() << std::endl;

                // Set input layer data
                layers[0].inputs_ = input_data;

                // Forward pass through all layers
                for(auto& layer : layers) {
                    layer.forward();
                }

            }


    private:
        std::vector<Layer> layers;

    };

} // namespace NN

