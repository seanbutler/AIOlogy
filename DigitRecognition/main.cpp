#include <iostream>
#include <string>
#include "libs/activations/activations.h"
#include "libs/layers/layers.h"

int main() {
    std::cout << "DigitRecognition Project\n";
    std::cout << "========================\n\n";
    
    std::cout << "Welcome to the Digit Recognition application!\n";
    std::cout << "This project uses C++23 features.\n\n";
    
    // Test the activations library
    activations::test_activations();
    
    std::cout << "\n";
    
    // Test the layers library
    layers::test_layers();
    
    std::cout << "\nTODO: Implement digit recognition functionality\n";

    return 0;
}