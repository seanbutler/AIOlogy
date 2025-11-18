#pragma once

#include <vector>
#include <string>
#include "../layers/layers.h"

namespace ANN {
namespace Visualization {

    // Save layer weights as a grid image
    // For a layer with input_size x output_size, creates a visualization
    // where each output neuron's weights are shown as a row/grid
    void save_weights_as_image(
        const std::vector<double>& weights,
        int input_size,
        int output_size,
        const std::string& filename,
        int grid_width = 28,  // For MNIST, typically 28x28
        int grid_height = 28,
        int scale = 2  // Scale factor for better visibility
    );

    // Save all network layers' weights
    void save_network_weights(
        const std::vector<ANN::Layer>& layers,
        const ANN::Layer& input_layer,
        const ANN::Layer& output_layer,
        const std::string& prefix = "weights"
    );

} // namespace Visualization
} // namespace ANN
