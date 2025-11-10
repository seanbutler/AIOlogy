#include <iostream>
#include <iomanip>
#include "AircraftPhysics.h"
#include "ConfigLoader.h"

void printCSVHeader() {
    std::cout << "Time,PosX,PosY,PosZ,VelX,VelY,VelZ,Speed,Altitude,Roll,Pitch,Yaw,Throttle,RPM,AirDensity" << std::endl;
}

void printStateCSV(const AircraftState& state, double time, double airDensity) {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << time << ","
              << state.position.x << ","
              << state.position.y << ","
              << state.position.z << ","
              << state.velocity.x << ","
              << state.velocity.y << ","
              << state.velocity.z << ","
              << state.velocity.magnitude() << ","
              << -state.position.z << ","
              << (state.roll * 180.0 / 3.14159) << ","
              << (state.pitch * 180.0 / 3.14159) << ","
              << (state.yaw * 180.0 / 3.14159) << ","
              << state.throttle << ","
              << state.currentRPM << ","
              << airDensity << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << "   Aircraft Physics Simulation" << std::endl;
    std::cout << "========================================" << std::endl;

    // Determine config file
    std::string configFile = "config.json";
    if (argc > 1) {
        configFile = argv[1];
    }
    
    // Load configuration from JSON
    AircraftSpec spec;
    AircraftState state;
    SimulationParameters simParams;
    Planet planet;
    
    std::cout << "\nLoading configuration from: " << configFile << std::endl;
    
    if (!ConfigLoader::loadFromFile(configFile, spec, state, simParams, planet)) {
        std::cerr << "Warning: Could not load config file. Using defaults." << std::endl;
        std::cerr << "Creating template config.json..." << std::endl;
        ConfigLoader::saveTemplate("config.json");
    } else {
        std::cout << "Configuration loaded successfully!" << std::endl;
    }
    
    std::cout << "\nPlanet Properties:" << std::endl;
    std::cout << "  Gravity: " << planet.gravity << " m/s^2" << std::endl;
    std::cout << "  Sea Level Density: " << planet.seaLevelDensity << " kg/m^3" << std::endl;
    std::cout << "  Temperature Lapse Rate: " << planet.tempLapsRate << " K/m" << std::endl;
    std::cout << "  Has Atmosphere: " << (planet.hasAtmosphere ? "Yes" : "No") << std::endl;
    
    std::cout << "\nAircraft Specifications:" << std::endl;
    std::cout << "  Mass: " << spec.mass << " kg" << std::endl;
    std::cout << "  Wing Area: " << spec.wingArea << " m^2" << std::endl;
    std::cout << "  Max Thrust: " << spec.maxThrust << " N" << std::endl;
    std::cout << "  RPM Range: " << spec.minRPM << " - " << spec.maxRPM << std::endl;
    
    std::cout << "\nInitial State:" << std::endl;
    std::cout << "  Position: " << state.position << std::endl;
    std::cout << "  Velocity: " << state.velocity << std::endl;
    std::cout << "  Throttle: " << (state.throttle * 100) << "%" << std::endl;
    std::cout << "  RPM: " << state.currentRPM << std::endl;
    
    std::cout << "\nSimulation Parameters:" << std::endl;
    std::cout << "  Timestep: " << simParams.dt << " s" << std::endl;
    std::cout << "  Total Time: " << simParams.totalTime << " s" << std::endl;
    std::cout << "  Print Interval: " << simParams.printInterval << " steps" << std::endl;
    std::cout << std::endl;

    // Create physics engine
    AircraftPhysics physics(spec, planet);

    // Print CSV header
    printCSVHeader();
    
    // Print initial state
    printStateCSV(state, 0.0, physics.getAirDensity(state));

    // Run simulation
    for (int step = 1; step <= static_cast<int>(simParams.totalTime / simParams.dt); ++step) {
        double currentTime = step * simParams.dt;
        
        // Simple autopilot: increase pitch slightly after gaining speed
        if (state.velocity.magnitude() > 30.0 && currentTime < 5.0) {
            state.elevator = 0.1;  // Slight nose-up
        }
        
        // Step the physics forward
        state = physics.step(state, simParams.dt);
        
        // Print state periodically
        if (step % simParams.printInterval == 0) {
            printStateCSV(state, currentTime, physics.getAirDensity(state));
        }
    }

    return 0;
}
