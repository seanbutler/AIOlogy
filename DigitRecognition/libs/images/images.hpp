#pragma once
#include <vector>
#include <string>

namespace ANN {

    // Load an image from the given file and return its pixel values as a vector of doubles
    std::vector<double> load_image(const std::string& filename);

} // namespace ANN
