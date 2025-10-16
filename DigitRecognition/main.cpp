#include <iostream>
#include <string>
#include "libs/activations/activations.h"
#include "libs/layers/layers.h"

int main() {
    std::cout << "DigitRecognition Project\n";
    std::cout << "========================\n\n";

    std::cout << "Welcome to the Digit Recognition application!\n";
    std::cout << "This application is a work in progress.\n\n";

    NN::Layer layer(16, 16);
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