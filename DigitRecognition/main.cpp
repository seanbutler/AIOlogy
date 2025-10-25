#include <iostream>
#include <string>
#include <filesystem>
#include <iomanip>
#include <random>
#include <algorithm>
#include <execution>
#include <atomic>
#include <chrono>
#include <fstream>


#include "libs/activations/activations.h"
#include "libs/layers/layers.h"
#include "libs/images/images.hpp"
#include "libs/networks/networks.hpp"
#include "libs/training/training.hpp"
#include "libs/config/config.hpp"

#include "utils.hpp"

#include "version.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {

    std::cout << "DigitRecognition v" << Version::VERSION_STRING << std::endl;
    std::cout << "Built: " << Version::BUILD_DATE << std::endl;
    std::cout << "Git: " << Version::GIT_COMMIT << std::endl << std::endl;
    std::cout << "Time: " << Utils::Time::HumanReadableTimeNowMillis() << std::endl << std::endl;

    // Load configuration from config.json
    ANN::Config config;
    if (!config.validate()) {
        std::cerr << "Invalid configuration, exiting." << std::endl;
        return 1;
    }
    
    // Print loaded configuration
    config.print();
    std::cout << std::endl;

    std::cout << "Time " << Utils::Time::HumanReadableTimeNowMillis() << std::endl << std::endl;

    // Create network from configuration
    ANN::WeightInitConfig weight_config;
    weight_config.method = config.network.weight_init.method;
    weight_config.range = config.network.weight_init.range;
    
    ANN::Network network(config.network.layers, config.network.learning_rate, weight_config);
    ANN::TrainingSet training_set;

    //
    // Load TRAINING data from train directory
    //
    
    std::cout << " Constructing Training Sets " << std::endl;

    for (const auto& entry : std::filesystem::directory_iterator(config.data.train_path)) {
        if (entry.is_regular_file()) {
            if ( std::filesystem::path(entry).extension() == ".png" ) {
                
                std::string filename = std::filesystem::path(entry).filename().string();
                int label = std::stoi(filename.substr(0, filename.find('_')));

                std::vector<double> image_data = ANN::load_image(std::filesystem::path(entry).string());
                
                if (config.data.normalize && !image_data.empty()) {
                    ANN::normalise_image(image_data, 255);
                }

                training_set.add_instance({image_data, label, filename});

                // std::cout << "Added training instance: " << training_set.get_instances().size()   
                //         << " file: " << filename 
                //         << " label: " << label 
                //         << " size: " << image_data.size() 
                //         << "\n";
            }
        }
    }

    std::cout << "\nTraining set constructed from data, size " << training_set.get_instances().size() << std::endl;

    // // Check data distribution
    // std::vector<int> label_counts(10, 0);
    // for (const auto& instance : training_set.get_instances()) {
    //     label_counts[instance.label]++;
    // }
    
    // std::cout << "Data distribution:" << std::endl;
    // for (int i = 0; i < 10; ++i) {
    //     std::cout << "Digit " << i << ": " << label_counts[i] << " samples" << std::endl;
    // }
    // std::cout << std::endl;

    std::cout << "Training network..." << std::endl;
    std::cout << "Time " << Utils::Time::HumanReadableTimeNowMillis() << std::endl << std::endl;

    // Shuffle the training data to prevent catastrophic forgetting
    auto instances = training_set.get_instances();
    std::random_device rd;
    std::mt19937 g(rd());
    
    // Train for multiple epochs
    std::cout << "Training for " << config.training.epochs << " epochs on " << instances.size() << " samples..." << std::endl;
    
    // Open loss tracking file if configured
    std::ofstream loss_file;
    if (config.output.save_plots) {
        loss_file.open(config.output.loss_file);
        loss_file << "epoch,total_loss,avg_loss,training_accuracy,samples\n";  // CSV header
        std::cout << "Loss tracking enabled - saving to: " << config.output.loss_file << std::endl;
    }
    
    for (int epoch = 0; epoch < config.training.epochs; ++epoch) {
        std::cout << "Epoch " << (epoch + 1) << "/" << config.training.epochs << ": \n";

        if (config.training.shuffle) {
            std::shuffle(instances.begin(), instances.end(), g);
        }
        
        int samples_processed = 0;
        double total_loss = 0.0;  // Track total loss for this epoch
        int correct_predictions = 0;  // Track training accuracy
        


        for (const auto & instance : instances) {
            double sample_loss = network.train(instance.input_data, instance.label);
            total_loss += sample_loss;  // Accumulate loss
            
            // Calculate accuracy on this sample
            int predicted_label = network.predict_label(instance.input_data);
            if (predicted_label == instance.label) {
                correct_predictions++;
            }
            
            samples_processed++;

            // Show progress every 100 samples for better performance
            if (samples_processed % 100 == 0) {
                double current_accuracy = (double)correct_predictions / samples_processed * 100.0;
                std::cout << "Progress: " << samples_processed << "/" << instances.size() 
                          << " Acc: " << std::fixed << std::setprecision(1) << current_accuracy << "% \r";
            }
        }
        
        // std::atomic<int> samples_processed{0};
        // std::for_each(std::execution::par, instances.begin(), instances.end(),
        //       [&](const auto& instance) {
        //             // Each thread processes different samples
        //             network.train(instance.input_data, instance.label);
        //             samples_processed.fetch_add(1);
        //       });

        // Calculate statistics for this epoch
        double avg_loss = total_loss / samples_processed;
        double training_accuracy = (double)correct_predictions / samples_processed * 100.0;
        
        std::cout << "  Epoch " << (epoch + 1) << " completed: " << samples_processed << " samples"
                  << " | Loss: " << std::fixed << std::setprecision(6) << avg_loss
                  << " | Train Acc: " << std::fixed << std::setprecision(2) << training_accuracy << "%" << std::endl;

        // Save loss data to CSV file
        if (config.output.save_plots && loss_file.is_open()) {
            loss_file << (epoch + 1) << "," << total_loss << "," << avg_loss << "," 
                      << training_accuracy << "," << samples_processed << "\n";
            loss_file.flush();  // Ensure data is written immediately
        }

        //
        // Output Loss Per Epoch
        //
        // double avg_loss = total_loss / samples_processed;
        // loss_file << epoch << "," << total_loss << ", " << avg_loss << "," << accuracy << "\n";
    }
    
    // Close loss file
    if (loss_file.is_open()) {
        loss_file.close();
        std::cout << "Loss data saved to: " << config.output.loss_file << std::endl;
    }

    std::cout << "Training completed!\n";

    std::cout << " Time " << Utils::Time::HumanReadableTimeNowMillis() << std::endl << std::endl;

    std::cout << "\n\nTesting network on test data..." << std::endl;

    int count = 0;
    int correct = 0;
    for (const auto& entry : std::filesystem::directory_iterator(config.data.test_path)) {
        if (entry.is_regular_file()) {
            if ( std::filesystem::path(entry).extension() == ".png" ) {
                
                std::string filename = std::filesystem::path(entry).filename().string();
                int label = std::stoi(filename.substr(0, filename.find('_')));

                std::vector<double> image_data = ANN::load_image(std::filesystem::path(entry).string());
                
                if (config.data.normalize && !image_data.empty()) {
                    ANN::normalise_image(image_data, 255);
                }

                //
                // test it 
                //
                int predicted = network.predict_label(image_data);
                auto raw_outputs = network.predict_probabilities(image_data);
                
                std::cout << "File: " << filename << " label: " << label << " predicted: " << predicted;

                count++;                
                if ( predicted == label ) {
                    std::cout << " correct\r";
                    correct++;
                } else {
                    std::cout << " incorrect\r";
                }
            }
        }
    }
    std::cout << std::endl;

    // Final accuracy summary
    std::cout << "\n=== FINAL RESULTS ===\n";
    std::cout << "Total tested: " << count << " images\n";
    std::cout << "Correct predictions: " << correct << "\n";
    std::cout << "Final accuracy: " << std::fixed << std::setprecision(2) << (static_cast<double>(correct) / static_cast<double>(count)) * 100.0 << "%\n";
    std::cout << "Time " << Utils::Time::HumanReadableTimeNowMillis() << std::endl << std::endl;

    return 0;
}