#pragma once
#include <vector>
#include <string>

namespace ANN {

    //
    // Load an image from the given file and return its pixel values as a vector of doubles
    //
    std::vector<double> load_image(const std::string& filename);

    //
    // Scale (normalise) the image data by dividing it by constant
    //
    void normalise_image(std::vector<double>& image_data, const double max_value=255);
    
} // namespace ANN
