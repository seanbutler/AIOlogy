#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../layers/layers.h"
#include "../learning_rate/learning_rate.hpp"

namespace ANN {
    struct OutputConfig {
        bool save_plots;
        std::string loss_file;
    };

    struct Config {
        struct NetworkConfig {
            std::vector<int> layers;
            std::string activation;
            ANN::WeightInitConfig weight_init;
        } network;

        struct TrainingConfig {
            int epochs;
            bool shuffle;
            std::string data_path;
            ANN::LearningRateConfig learning_rate;
        } training;

        struct DataConfig {
            std::string train_path;
            std::string test_path;
            std::vector<int> image_size;
            bool normalize;
        } data;

        OutputConfig output;

        Config(const std::string& config_file = "config.json");
        void load_from_file(const std::string& config_file);
        void save_to_file(const std::string& config_file) const;
        void load_defaults();
        void print() const;
        bool validate() const;
    };
}
