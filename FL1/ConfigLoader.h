#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include "AircraftSpec.h"
#include "AircraftState.h"
#include "Vec3.h"
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

struct SimulationParameters {
    double dt;              // Timestep in seconds
    double totalTime;       // Total simulation time in seconds
    int printInterval;      // Print every N steps
    
    SimulationParameters() : dt(0.01), totalTime(10.0), printInterval(100) {}
};

class ConfigLoader {
public:
    static bool loadFromFile(const std::string& filename, 
                            AircraftSpec& spec, 
                            AircraftState& state,
                            SimulationParameters& simParams) {
        try {
            std::ifstream file(filename);
            if (!file.is_open()) {
                return false;
            }
            
            json j;
            file >> j;
            
            // Load aircraft spec from nested object
            if (j.contains("aircraftSpec")) {
                auto& specJson = j["aircraftSpec"];
                spec.mass = specJson.value("mass", spec.mass);
                spec.wingArea = specJson.value("wingArea", spec.wingArea);
                spec.wingSpan = specJson.value("wingSpan", spec.wingSpan);
                spec.liftCoeff = specJson.value("liftCoeff", spec.liftCoeff);
                spec.dragCoeff = specJson.value("dragCoeff", spec.dragCoeff);
                spec.parasiteDrag = specJson.value("parasiteDrag", spec.parasiteDrag);
                spec.inducedDragFactor = specJson.value("inducedDragFactor", spec.inducedDragFactor);
                spec.maxThrust = specJson.value("maxThrust", spec.maxThrust);
                spec.thrustAngle = specJson.value("thrustAngle", spec.thrustAngle);
                spec.minRPM = specJson.value("minRPM", spec.minRPM);
                spec.maxRPM = specJson.value("maxRPM", spec.maxRPM);
                spec.rpmSpoolUpRate = specJson.value("rpmSpoolUpRate", spec.rpmSpoolUpRate);
                spec.rpmSpoolDownRate = specJson.value("rpmSpoolDownRate", spec.rpmSpoolDownRate);
                spec.inertiaX = specJson.value("inertiaX", spec.inertiaX);
                spec.inertiaY = specJson.value("inertiaY", spec.inertiaY);
                spec.inertiaZ = specJson.value("inertiaZ", spec.inertiaZ);
            }
            
            // Load initial state from nested object
            if (j.contains("aircraftState")) {
                auto& stateJson = j["aircraftState"];
                
                if (stateJson.contains("position")) {
                    state.position.x = stateJson["position"].value("x", state.position.x);
                    state.position.y = stateJson["position"].value("y", state.position.y);
                    state.position.z = stateJson["position"].value("z", state.position.z);
                }
                
                if (stateJson.contains("velocity")) {
                    state.velocity.x = stateJson["velocity"].value("x", state.velocity.x);
                    state.velocity.y = stateJson["velocity"].value("y", state.velocity.y);
                    state.velocity.z = stateJson["velocity"].value("z", state.velocity.z);
                }
                
                if (stateJson.contains("orientation")) {
                    state.roll = stateJson["orientation"].value("roll", state.roll);
                    state.pitch = stateJson["orientation"].value("pitch", state.pitch);
                    state.yaw = stateJson["orientation"].value("yaw", state.yaw);
                }
                
                if (stateJson.contains("controls")) {
                    state.throttle = stateJson["controls"].value("throttle", state.throttle);
                    state.elevator = stateJson["controls"].value("elevator", state.elevator);
                    state.aileron = stateJson["controls"].value("aileron", state.aileron);
                    state.rudder = stateJson["controls"].value("rudder", state.rudder);
                }
                
                state.currentRPM = stateJson.value("currentRPM", state.currentRPM);
            }
            
            // Load simulation parameters from nested object
            if (j.contains("simulationParameters")) {
                auto& simJson = j["simulationParameters"];
                simParams.dt = simJson.value("dt", simParams.dt);
                simParams.totalTime = simJson.value("totalTime", simParams.totalTime);
                simParams.printInterval = simJson.value("printInterval", simParams.printInterval);
            }
            
            return true;
        } catch (const std::exception& e) {
            return false;
        }
    }
    
    static void saveTemplate(const std::string& filename) {
        json j = {
            {"aircraftSpec", {
                {"mass", 1200.0},
                {"wingArea", 16.0},
                {"wingSpan", 10.0},
                {"liftCoeff", 0.5},
                {"parasiteDrag", 0.03},
                {"inducedDragFactor", 0.045},
                {"maxThrust", 6000.0},
                {"thrustAngle", 0.0},
                {"minRPM", 800.0},
                {"maxRPM", 2700.0},
                {"rpmSpoolUpRate", 500.0},
                {"rpmSpoolDownRate", 300.0},
                {"inertiaX", 500.0},
                {"inertiaY", 800.0},
                {"inertiaZ", 1200.0}
            }},
            {"aircraftState", {
                {"position", {
                    {"x", 0.0},
                    {"y", 0.0},
                    {"z", 0.0}
                }},
                {"velocity", {
                    {"x", 20.0},
                    {"y", 0.0},
                    {"z", 0.0}
                }},
                {"orientation", {
                    {"roll", 0.0},
                    {"pitch", 0.0},
                    {"yaw", 0.0}
                }},
                {"controls", {
                    {"throttle", 0.7},
                    {"elevator", 0.0},
                    {"aileron", 0.0},
                    {"rudder", 0.0}
                }},
                {"currentRPM", 800.0}
            }},
            {"simulationParameters", {
                {"dt", 0.1},
                {"totalTime", 100.0},
                {"printInterval", 100}
            }}
        };
        
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not create config file");
        }
        
        file << j.dump(2) << std::endl;
    }
};

#endif // CONFIG_LOADER_H
