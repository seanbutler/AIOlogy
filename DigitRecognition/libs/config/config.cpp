// Includes
#include "config.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

namespace ANN {

void Config::load_from_file(const std::string& config_file) {
    try {
        std::ifstream file(config_file);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open config file: " + config_file);
        }

        nlohmann::json config_json;
        file >> config_json;

        // Parse network configuration
        if (config_json.contains("network")) {
            auto net = config_json["network"];
            network.layers = net.value("layers", std::vector<int>{784, 128, 64, 10});
            network.activation = net.value("activation", "sigmoid");
            // Parse weight initialization
            if (net.contains("weight_init")) {
                auto weight_init = net["weight_init"];
                network.weight_init.method = weight_init.value("method", "uniform");
                network.weight_init.range = weight_init.value("range", std::vector<double>{-1.0, 1.0});
            } else {
                network.weight_init.method = "uniform";
                network.weight_init.range = {-1.0, 1.0};
            }
        }

        // Parse training configuration
        if (config_json.contains("training")) {
            auto train = config_json["training"];
            training.epochs = train.value("epochs", 5);
            training.shuffle = train.value("shuffle", true);
            training.data_path = train.value("data_path", "./data/mnist_images/");
            // Parse learning rate schedule
            if (train.contains("learning_rate")) {
                training.learning_rate = ANN::LearningRateConfig::from_json(train["learning_rate"]);
            } else {
                training.learning_rate = ANN::LearningRateConfig();
            }
        }

        // Parse data configuration
        if (config_json.contains("data")) {
            auto data_config = config_json["data"];
            data.train_path = data_config.value("train_path", "./data/mnist_images/train/");
            data.test_path = data_config.value("test_path", "./data/mnist_images/test/");
            data.image_size = data_config.value("image_size", std::vector<int>{28, 28});
            data.normalize = data_config.value("normalize", true);
        }

        // Parse output configuration
        if (config_json.contains("output")) {
            auto output_config = config_json["output"];
            output.save_plots = output_config.value("save_plots", true);
            output.loss_file = output_config.value("loss_file", "training_loss.csv");
        }

    } catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
        load_defaults();
    }
}

void Config::save_to_file(const std::string& config_file) const {
    nlohmann::json config_json;
    // Network configuration
    config_json["network"]["layers"] = network.layers;
    config_json["network"]["activation"] = network.activation;
    config_json["network"]["weight_init"]["method"] = network.weight_init.method;
    config_json["network"]["weight_init"]["range"] = network.weight_init.range;
    // Training configuration
    config_json["training"]["epochs"] = training.epochs;
    config_json["training"]["shuffle"] = training.shuffle;
    config_json["training"]["data_path"] = training.data_path;
    config_json["training"]["learning_rate"] = {
        {"initial", training.learning_rate.initial},
        {"schedule", training.learning_rate.schedule},
        {"decay", training.learning_rate.decay},
        {"min", training.learning_rate.min},
        {"step", training.learning_rate.step}
    };
    // Data configuration
    config_json["data"]["train_path"] = data.train_path;
    config_json["data"]["test_path"] = data.test_path;
    config_json["data"]["image_size"] = data.image_size;
    config_json["data"]["normalize"] = data.normalize;
    // Output configuration
    config_json["output"]["save_plots"] = output.save_plots;
    config_json["output"]["loss_file"] = output.loss_file;
    std::ofstream file(config_file);
    file << config_json.dump(2);  // Pretty print with 2-space indentation
}

void Config::load_defaults() {
    network.layers = {784, 128, 64, 10};
    network.activation = "sigmoid";
    network.weight_init.method = "uniform";
    network.weight_init.range = {-1.0, 1.0};
    training.epochs = 5;
    training.shuffle = true;
    training.data_path = "./data/mnist_images/";
    training.learning_rate = ANN::LearningRateConfig();
    data.train_path = "./data/mnist_images/train/";
    data.test_path = "./data/mnist_images/test/";
    data.image_size = {28, 28};
    data.normalize = true;
    output.save_plots = true;
    output.loss_file = "training_loss.csv";
}

Config::Config(const std::string& config_file) {
    load_defaults();
    load_from_file(config_file);
}

void Config::print() const {
    std::cout << "=== CONFIGURATION ===" << std::endl;
    std::cout << "Network:" << std::endl;
    std::cout << "\tLayers:\t[";
    for (size_t i = 0; i < network.layers.size(); ++i) {
        std::cout << network.layers[i];
        if (i < network.layers.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    std::cout << "\tActivation:\t" << network.activation << std::endl;
    std::cout << "\tWeight Init:\t" << network.weight_init.method << " (" << network.weight_init.range[0] << ", " << network.weight_init.range[1] << ")" << std::endl;
    std::cout << "Training:" << std::endl;
    std::cout << "\tEpochs:\t" << training.epochs << std::endl;
    std::cout << "\tShuffle:\t" << (training.shuffle ? "true" : "false") << std::endl;
    std::cout << "\tData Path:\t" << training.data_path << std::endl;
    std::cout << "\tLearning Rate Initial:\t" << training.learning_rate.initial << std::endl;
    std::cout << "\tLearning Rate Schedule:\t" << training.learning_rate.schedule << std::endl;
    std::cout << "\tLearning Rate Decay:\t" << training.learning_rate.decay << std::endl;
    std::cout << "\tLearning Rate Min:\t" << training.learning_rate.min << std::endl;
    std::cout << "\tLearning Rate Step:\t" << training.learning_rate.step << std::endl;
    std::cout << "Data:" << std::endl;
    std::cout << "\tTrain Path:\t" << data.train_path << std::endl;
    std::cout << "\tTest Path:\t" << data.test_path << std::endl;
    std::cout << "\tImage Size:\t" << data.image_size[0] << "x" << data.image_size[1] << std::endl;
    std::cout << "\tNormalize:\t" << (data.normalize ? "true" : "false") << std::endl;
    std::cout << "=====================" << std::endl;
}

bool Config::validate() const {
    if (network.layers.size() < 2) {
        std::cerr << "Error: Network must have at least 2 layers" << std::endl;
        return false;
    }
    if (training.learning_rate.initial <= 0.0 || training.learning_rate.initial > 1.0) {
        std::cerr << "Error: Initial learning rate must be between 0 and 1" << std::endl;
        return false;
    }
    if (training.epochs <= 0) {
        std::cerr << "Error: Epochs must be positive" << std::endl;
        return false;
    }
    return true;
}
// End of namespace ANN




}




