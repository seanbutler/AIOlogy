#include <iostream>
#include <string>
#include "libs/activations/activations.h"
#include "libs/layers/layers.h"
#include "version.h"

int main() {
    std::cout << "DigitRecognition v" << Version::VERSION_STRING << std::endl;
    std::cout << "Built: " << Version::BUILD_DATE << std::endl;
    std::cout << "Git: " << Version::GIT_COMMIT << std::endl;
    
    ANN::Layer layer(16, 16);
    layer.inputs_ = {1.0, 0.5, 0.25, 0.75, 0.1, 0.9, 0.33, 0.66, 0.2, 0.8, 0.4, 0.6, 0.15, 0.85, 0.05, 0.95};
    layer.forward();

    std::cout << "\n\n";
    
    std::cout << "Output: ";
    for ( auto out : layer.outputs_) {
        std::cout << out << " ";
    }

    std::cout << "\n\n";

    std::cout << "\nTODO: Implement digit recognition functionality\n";

    return 0;
}