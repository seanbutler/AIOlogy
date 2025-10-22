
#include <iostream>
#include <string>
#include <filesystem>

#include "libs/activations/activations.h"
#include "libs/layers/layers.h"
#include "libs/images/images.hpp"
#include "libs/networks/networks.hpp"
#include "libs/training/training.hpp"

#include "version.h"

int main(int argc, char** argv) {
    std::cout << "DigitRecognition v" << Version::VERSION_STRING << std::endl;
    std::cout << "Built: " << Version::BUILD_DATE << std::endl;
    std::cout << "Git: " << Version::GIT_COMMIT << std::endl << std::endl;
    
    ANN::Network network({784, 32, 16, 10});
    ANN::TrainingSet training_set;

    for (const auto& entry : std::filesystem::directory_iterator("./data/mnist_images/train/")) {
        if (entry.is_regular_file()) {
            if ( std::filesystem::path(entry).extension() == ".png" ) {
                
                std::string filename = std::filesystem::path(entry).filename().string();
                int label = std::stoi(filename.substr(0, filename.find('_')));

                std::vector<double> image_data = ANN::load_image(std::filesystem::path(entry).string());

                training_set.add_instance({image_data, label, filename});

                std::cout << "Added training instance: " << training_set.get_instances().size() << " "  
                          << "file: " << filename << " label: " << label << " size: " << image_data.size() << "\r";
            }
        }
    }

    std::cout << "\nTraining set constructed from data, size " << training_set.get_instances().size() << std::endl;

    for (const auto & instance : training_set.get_instances()) {
        network.train(instance.input_data, instance.label);
    }

    return 0;
}