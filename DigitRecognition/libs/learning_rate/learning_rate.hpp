#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace ANN {
    struct LearningRateConfig {
        double initial = 0.01;
        std::string schedule = "constant"; // "constant", "exponential", "linear"
        double decay = 0.0; // Used for exponential/linear
        double min = 0.0001; // Minimum learning rate
        int step = 1; // Step interval for schedule update
        int epoch = 0; // Current epoch

        LearningRateConfig() = default;
        LearningRateConfig(double initial_, const std::string& schedule_, double decay_, double min_, int step_)
            : initial(initial_), schedule(schedule_), decay(decay_), min(min_), step(step_), epoch(0) {}

        double get() const {
            return initial;
        }

        void update(int current_epoch) {
            epoch = current_epoch;
            if (schedule == "constant") {
                // No change
            } else if (schedule == "exponential") {
                initial = std::max(min, initial * std::exp(-decay * epoch));
            } else if (schedule == "linear") {
                initial = std::max(min, initial - decay * epoch);
            }
        }

        static LearningRateConfig from_json(const nlohmann::json& j) {
            LearningRateConfig cfg;
            if (j.contains("initial")) cfg.initial = j["initial"].get<double>();
            if (j.contains("schedule")) cfg.schedule = j["schedule"].get<std::string>();
            if (j.contains("decay")) cfg.decay = j["decay"].get<double>();
            if (j.contains("min")) cfg.min = j["min"].get<double>();
            if (j.contains("step")) cfg.step = j["step"].get<int>();
            return cfg;
        }
    };
}
