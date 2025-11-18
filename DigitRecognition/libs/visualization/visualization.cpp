#include "visualization.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <fstream>

#if HAS_SDL_IMAGE
#include <SDL.h>
#include <SDL_image.h>
#endif

namespace ANN {
namespace Visualization {

void save_weights_as_image(
    const std::vector<double>& weights,
    int input_size,
    int output_size,
    const std::string& filename,
    int grid_width,
    int grid_height,
    int scale
) {
#if HAS_SDL_IMAGE
    // Calculate how many neurons we can fit per row
    // Each neuron is visualized as a grid_width x grid_height image
    int neurons_per_row = std::min(10, output_size);  // Max 10 neurons per row
    int num_rows = (output_size + neurons_per_row - 1) / neurons_per_row;
    
    int cell_width = grid_width * scale;
    int cell_height = grid_height * scale;
    int spacing = 2;  // Pixels between neurons
    
    int image_width = neurons_per_row * (cell_width + spacing) + spacing;
    int image_height = num_rows * (cell_height + spacing) + spacing;
    
    // Create SDL surface
    SDL_Surface* surface = SDL_CreateRGBSurface(
        0, image_width, image_height, 32,
        0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
    );
    
    if (!surface) {
        std::cerr << "Failed to create SDL surface: " << SDL_GetError() << std::endl;
        return;
    }
    
    // Fill with gray background
    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 128, 128, 128));
    
    // Find min and max weights for normalization
    double min_weight = *std::min_element(weights.begin(), weights.end());
    double max_weight = *std::max_element(weights.begin(), weights.end());
    double range = std::max(std::abs(min_weight), std::abs(max_weight));
    
    if (range < 1e-10) range = 1.0;  // Avoid division by zero
    
    // Lock surface for pixel access
    if (SDL_MUSTLOCK(surface)) {
        SDL_LockSurface(surface);
    }
    
    // Draw each neuron's weights
    for (int neuron = 0; neuron < output_size; ++neuron) {
        int row = neuron / neurons_per_row;
        int col = neuron % neurons_per_row;
        
        int base_x = spacing + col * (cell_width + spacing);
        int base_y = spacing + row * (cell_height + spacing);
        
        // Draw this neuron's weights as a grid
        for (int i = 0; i < input_size && i < grid_width * grid_height; ++i) {
            int weight_idx = i + (neuron * input_size);
            double weight = weights[weight_idx];
            
            // Normalize weight to [-1, 1] then to [0, 255]
            double normalized = weight / range;
            
            // Color scheme: blue for negative, red for positive, white for zero
            Uint8 r, g, b;
            if (normalized > 0) {
                // Positive weights: white to red
                r = 255;
                g = static_cast<Uint8>(255 * (1.0 - normalized));
                b = static_cast<Uint8>(255 * (1.0 - normalized));
            } else {
                // Negative weights: white to blue
                r = static_cast<Uint8>(255 * (1.0 + normalized));
                g = static_cast<Uint8>(255 * (1.0 + normalized));
                b = 255;
            }
            
            // Calculate position in grid
            int grid_x = i % grid_width;
            int grid_y = i / grid_width;
            
            // Draw scaled pixel
            SDL_Rect rect;
            rect.x = base_x + grid_x * scale;
            rect.y = base_y + grid_y * scale;
            rect.w = scale;
            rect.h = scale;
            
            SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, r, g, b));
        }
    }
    
    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }
    
    // Save as PNG
    if (IMG_SavePNG(surface, filename.c_str()) != 0) {
        std::cerr << "Failed to save image: " << IMG_GetError() << std::endl;
    } else {
        std::cout << "Saved weight visualization: " << filename << std::endl;
    }
    
    SDL_FreeSurface(surface);
#else
    std::cerr << "SDL2_image not available - cannot save weight visualization" << std::endl;
#endif
}

void save_network_weights(
    const std::vector<ANN::Layer>& layers,
    const ANN::Layer& input_layer,
    const ANN::Layer& output_layer,
    const std::string& prefix
) {
    // Save input layer weights (784 inputs -> first hidden layer)
    int input_size = static_cast<int>(input_layer.inputs_.size());
    int input_output_size = static_cast<int>(input_layer.outputs_.size());
    
    std::string filename = prefix + "_layer_0_input.png";
    save_weights_as_image(
        input_layer.weights_,
        input_size,
        input_output_size,
        filename,
        28, 28, 2
    );
    
    // Save hidden layers
    for (size_t i = 0; i < layers.size(); ++i) {
        int layer_input_size = static_cast<int>(layers[i].inputs_.size());
        int layer_output_size = static_cast<int>(layers[i].outputs_.size());
        
        filename = prefix + "_layer_" + std::to_string(i + 1) + "_hidden.png";
        
        // For hidden layers, we can't visualize as 28x28 grids
        // Instead, show as horizontal strips
        int grid_size = static_cast<int>(std::sqrt(layer_input_size));
        if (grid_size * grid_size < layer_input_size) grid_size++;
        
        save_weights_as_image(
            layers[i].weights_,
            layer_input_size,
            layer_output_size,
            filename,
            grid_size, grid_size, 1
        );
    }
    
    // Save output layer weights
    int output_input_size = static_cast<int>(output_layer.inputs_.size());
    int output_output_size = static_cast<int>(output_layer.outputs_.size());
    
    filename = prefix + "_layer_" + std::to_string(layers.size() + 1) + "_output.png";
    
    int grid_size = static_cast<int>(std::sqrt(output_input_size));
    if (grid_size * grid_size < output_input_size) grid_size++;
    
    save_weights_as_image(
        output_layer.weights_,
        output_input_size,
        output_output_size,
        filename,
        grid_size, grid_size, 2
    );
}

} // namespace Visualization
} // namespace ANN
