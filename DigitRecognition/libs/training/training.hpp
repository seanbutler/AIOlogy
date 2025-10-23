#pragma once

#include <vector>
#include <string>


namespace ANN {

    struct TrainingInstance {
        std::vector<double> input_data;
        int label;
        std::string filename;
    };


    class TrainingSet {
    public:
        TrainingSet() = default;
        ~TrainingSet() = default;

        void add_instance(const TrainingInstance& instance) {
            instances_.push_back(instance);
        }

        const std::vector<TrainingInstance>& get_instances() const {
            return instances_;
        }

    private:
        std::vector<TrainingInstance> instances_;
    };

    
}