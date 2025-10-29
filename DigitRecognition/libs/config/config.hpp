#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using json = nlohmann::json;

namespace ANN {

class Config {
public:
    // Network configuration
    struct NetworkConfig {
        std::vector<int> layers;
        std::string activation;
        
        // Weight initialization
        struct WeightInit {
            std::string method;     // "uniform", "normal", "xavier", "he"
            std::vector<double> range;  // [min, max] for uniform, [mean, std] for normal
        } weight_init;
    } network;

    // Training configuration
    struct TrainingConfig {
        int epochs;
        bool shuffle;
        std::string data_path;

        struct LearningRate {
            std::string method; // e.g. "exponential", "step", "constant"
            std::vector<double> values; // e.g. [initial, decay] or [initial, step, ...]
        } learning_rate;
    } training;

    // Data configuration
    struct DataConfig {
        std::string train_path;
        std::string test_path;
        std::vector<int> image_size;
        bool normalize;
    } data;

    // Output configuration
    struct OutputConfig {
        bool save_plots;
        std::string loss_file;
    } output;

    // Constructor
    Config(const std::string& config_file = "config.json") {
        load_from_file(config_file);
    }

    // Load configuration from JSON file
    void load_from_file(const std::string& config_file) {
        try {
            std::ifstream file(config_file);
            if (!file.is_open()) {
                throw std::runtime_error("Could not open config file: " + config_file);
            }

            json config_json;
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
                    // Default values
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
                    auto lr = train["learning_rate"];
                    training.learning_rate.method = lr.value("method", "constant");
                    training.learning_rate.values = lr.value("values", std::vector<double>{0.01});
                } else {
                    training.learning_rate.method = "constant";
                    training.learning_rate.values = {0.01};
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

    // Save current configuration to JSON file
    void save_to_file(const std::string& config_file) const {
        json config_json;

        // Network configuration
    config_json["network"]["layers"] = network.layers;
    config_json["network"]["activation"] = network.activation;
    config_json["network"]["weight_init"]["method"] = network.weight_init.method;
    config_json["network"]["weight_init"]["range"] = network.weight_init.range;

        // Training configuration
    config_json["training"]["epochs"] = training.epochs;
    config_json["training"]["shuffle"] = training.shuffle;
    config_json["training"]["data_path"] = training.data_path;
    config_json["training"]["learning_rate"]["method"] = training.learning_rate.method;
    config_json["training"]["learning_rate"]["values"] = training.learning_rate.values;

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

    // Load default configuration
    void load_defaults() {
        // Network defaults
        network.layers = {784, 128, 64, 10};
    // network.learning_rate removed
        network.activation = "sigmoid";
        network.weight_init.method = "uniform";
        network.weight_init.range = {-1.0, 1.0};

        // Training defaults
        training.epochs = 5;
        training.shuffle = true;
        training.data_path = "./data/mnist_images/";

        // Data defaults
        data.train_path = "./data/mnist_images/train/";
        data.test_path = "./data/mnist_images/test/";
        data.image_size = {28, 28};
        data.normalize = true;

        // Output defaults
        output.save_plots = true;
        output.loss_file = "training_loss.csv";
    }

        // Print current configuration
        void print() const {
            // TODO why not just pretty print the json object?
            std::cout << "=== CONFIGURATION ===" << std::endl;
            std::cout << "Network:" << std::endl;
            std::cout << "\tLayers:\t[";
            for (size_t i = 0; i < network.layers.size(); ++i) {
                std::cout << network.layers[i];
                if (i < network.layers.size() - 1) std::cout << ", ";
            }
            std::cout << "]" << std::endl;
            std::cout << "\tActivation:\t" << network.activation << std::endl;

            std::cout << "Training:" << std::endl;
            std::cout << "\tEpochs:\t" << training.epochs << std::endl;
            std::cout << "\tShuffle:\t" << (training.shuffle ? "true" : "false") << std::endl;
            std::cout << "\tData Path:\t" << training.data_path << std::endl;
            std::cout << "\tLearning Rate Method:\t" << training.learning_rate.method << std::endl;
            std::cout << "\tLearning Rate Values:\t[";
            for (size_t i = 0; i < training.learning_rate.values.size(); ++i) {
                std::cout << training.learning_rate.values[i];
                if (i < training.learning_rate.values.size() - 1) std::cout << ", ";
            }
            std::cout << "]" << std::endl;

            std::cout << "Data:" << std::endl;
            std::cout << "\tTrain Path:\t" << data.train_path << std::endl;
            std::cout << "\tTest Path:\t" << data.test_path << std::endl;
            std::cout << "\tImage Size:\t" << data.image_size[0] << "x" << data.image_size[1] << std::endl;
            std::cout << "\tNormalize:\t" << (data.normalize ? "true" : "false") << std::endl;
            std::cout << "=====================" << std::endl;
        }

    // Validation methods
    bool validate() const {
        if (network.layers.size() < 2) {
            std::cerr << "Error: Network must have at least 2 layers" << std::endl;
            return false;
        }
        // Validate learning rate values
        if (training.learning_rate.values.empty() || training.learning_rate.values[0] <= 0.0 || training.learning_rate.values[0] > 1.0) {
            std::cerr << "Error: Initial learning rate must be between 0 and 1" << std::endl;
            return false;
        }
        if (training.epochs <= 0) {
            std::cerr << "Error: Epochs must be positive" << std::endl;
            return false;
        }
        return true;
    }
};

} // namespace ANN