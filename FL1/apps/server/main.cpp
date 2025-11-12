#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <FlightSim/AircraftPhysics.h>
#include <FlightSim/ConfigLoader.h>
#include <FlightSim/NetworkServer.h>

// Global state with thread-safe access
std::mutex state_mutex;
AircraftState global_state;
double global_time = 0.0;

int main(int argc, char* argv[]) {
    try {
        std::cout << "========================================" << std::endl;
        std::cout << "   Flight Simulation Network Server" << std::endl;
        std::cout << "========================================" << std::endl;

    // Determine config file
    std::string configFile = "config.json";
    if (argc > 1) {
        configFile = argv[1];
    }
    
    // Determine port
    unsigned short port = 9090;
    if (argc > 2) {
        port = static_cast<unsigned short>(std::stoi(argv[2]));
    }
    
    // Load configuration from JSON
    AircraftSpec spec;
    AircraftState state;
    SimulationParameters simParams;
    Planet planet;
    
    std::cout << "\nLoading configuration from: " << configFile << std::endl;
    
    if (!ConfigLoader::loadFromFile(configFile, spec, state, simParams, planet)) {
        std::cerr << "Warning: Could not load config file. Using defaults." << std::endl;
        ConfigLoader::saveTemplate("config.json");
    } else {
        std::cout << "Configuration loaded successfully!" << std::endl;
    }
    
    std::cout << "\nSimulation Parameters:" << std::endl;
    std::cout << "  Timestep: " << simParams.dt << " s" << std::endl;
    std::cout << "  Network Port: " << port << std::endl;
    
    std::cout << "\nPlanet: " << std::endl;
    std::cout << "  Gravity: " << planet.gravity << " m/s^2" << std::endl;
    
    std::cout << "\nAircraft:" << std::endl;
    std::cout << "  Mass: " << spec.mass << " kg" << std::endl;
    std::cout << "  Max Thrust: " << spec.maxThrust << " N" << std::endl;
    
    // Initialize global state
    {
        std::lock_guard<std::mutex> lock(state_mutex);
        global_state = state;
    }
    
    // Create physics engine
    AircraftPhysics physics(spec, planet);
    
    // Create network server
    asio::io_context io_context;
    NetworkServer server(io_context, port);
    
    // Set up control command callback
    server.setControlCommandCallback(
        [](double throttle, double elevator, double aileron, double rudder) {
            try {
                std::lock_guard<std::mutex> lock(state_mutex);
                global_state.throttle = std::max(0.0, std::min(1.0, throttle));
                global_state.elevator = std::max(-1.0, std::min(1.0, elevator));
                global_state.aileron = std::max(-1.0, std::min(1.0, aileron));
                global_state.rudder = std::max(-1.0, std::min(1.0, rudder));
                std::cout << "Control update - Throttle: " << global_state.throttle 
                          << ", Elevator: " << global_state.elevator << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error in control callback: " << e.what() << std::endl;
            }
        });
    
    // Set up state query callback
    server.setQueryStateCallback([&physics]() -> json {
        try {
            std::lock_guard<std::mutex> lock(state_mutex);
            
            json j;
            j["type"] = "state";
            j["time"] = global_time;
            j["position"] = {
                {"x", global_state.position.x},
                {"y", global_state.position.y},
                {"z", global_state.position.z}
            };
            j["velocity"] = {
                {"x", global_state.velocity.x},
                {"y", global_state.velocity.y},
                {"z", global_state.velocity.z}
            };
            j["orientation"] = {
                {"roll", global_state.roll},
                {"pitch", global_state.pitch},
                {"yaw", global_state.yaw}
            };
            j["rpm"] = global_state.currentRPM;
            j["altitude"] = -global_state.position.z;
            j["airspeed"] = global_state.velocity.magnitude();
            j["airDensity"] = physics.getAirDensity(global_state);
            
            return j;
        } catch (const std::exception& e) {
            std::cerr << "Error in query callback: " << e.what() << std::endl;
            json error;
            error["type"] = "error";
            error["message"] = "Internal server error";
            return error;
        }
    });
    
    // Start network thread
    std::thread network_thread([&io_context]() {
        io_context.run();
    });
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "Server running. Press Ctrl+C to stop." << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // Physics simulation loop
    auto last_broadcast = std::chrono::steady_clock::now();
    const double broadcast_interval = 0.1; // 10 Hz
    
    std::atomic<bool> running{true};
    
    while (running) {
        auto loop_start = std::chrono::steady_clock::now();
        
        // Step physics (thread-safe)
        {
            std::lock_guard<std::mutex> lock(state_mutex);
            global_state = physics.step(global_state, simParams.dt);
            global_time += simParams.dt;
        }
        
        // Broadcast state at fixed interval
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = now - last_broadcast;
        
        if (elapsed.count() >= broadcast_interval) {
            std::lock_guard<std::mutex> lock(state_mutex);
            server.broadcastState(global_state, global_time, physics.getAirDensity(global_state));
            last_broadcast = now;
        }
        
        // Sleep to maintain timestep
        auto loop_end = std::chrono::steady_clock::now();
        std::chrono::duration<double> loop_duration = loop_end - loop_start;
        double sleep_time = simParams.dt - loop_duration.count();
        
        if (sleep_time > 0) {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(sleep_time));
        }
    }
    
    // Cleanup
    io_context.stop();
    network_thread.join();
    
    return 0;
    
    } catch (const std::exception& e) {
        std::cerr << "\n========================================" << std::endl;
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        std::cerr << "========================================" << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n========================================" << std::endl;
        std::cerr << "FATAL ERROR: Unknown exception" << std::endl;
        std::cerr << "========================================" << std::endl;
        return 2;
    }
}
