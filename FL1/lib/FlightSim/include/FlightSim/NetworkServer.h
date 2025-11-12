#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include <asio.hpp>
#include <nlohmann/json.hpp>
#include "AircraftState.h"
#include <memory>
#include <string>
#include <functional>
#include <iostream>
#include <sstream>

using json = nlohmann::json;

// Callback types for command handling
using ControlCommandCallback = std::function<void(double throttle, double elevator, double aileron, double rudder)>;
using QueryCommandCallback = std::function<json()>;

class NetworkServer {
private:
    asio::io_context& io_context_;
    asio::ip::tcp::acceptor acceptor_;
    asio::ip::tcp::socket socket_;
    
    // Callbacks
    ControlCommandCallback on_control_command_;
    QueryCommandCallback on_query_state_;
    
    // Buffer for receiving data
    asio::streambuf receive_buffer_;
    
    // Connection state
    bool client_connected_;
    
    void startAccept() {
        // Close socket if it's still open from previous connection
        if (socket_.is_open()) {
            asio::error_code ec;
            socket_.close(ec);
        }
        
        acceptor_.async_accept(socket_,
            [this](const asio::error_code& error) {
                if (!error) {
                    client_connected_ = true;
                    std::cout << "Client connected from: " 
                              << socket_.remote_endpoint() << std::endl;
                    
                    // Send welcome message
                    sendWelcomeMessage();
                    
                    // Start receiving data
                    startReceive();
                } else {
                    std::cerr << "Accept error: " << error.message() << std::endl;
                    // Try to accept again after error
                    startAccept();
                }
            });
    }
    
    void startReceive() {
        asio::async_read_until(socket_, receive_buffer_, '\n',
            [this](const asio::error_code& error, std::size_t bytes_transferred) {
                if (!error) {
                    try {
                        // Extract the message safely with error checking
                        std::string message;
                        try {
                            std::istream is(&receive_buffer_);
                            if (is.good()) {
                                std::getline(is, message);
                            } else {
                                std::cerr << "Stream not in good state" << std::endl;
                                return;
                            }
                        } catch (const std::exception& e) {
                            std::cerr << "Error reading from buffer: " << e.what() << std::endl;
                            client_connected_ = false;
                            asio::error_code ec;
                            socket_.close(ec);
                            startAccept();
                            return;
                        }
                        
                        // Safely remove trailing characters
                        try {
                            while (!message.empty() && (message.back() == '\r' || message.back() == '\n')) {
                                message.pop_back();
                            }
                        } catch (const std::exception& e) {
                            std::cerr << "Error cleaning message: " << e.what() << std::endl;
                            message.clear();
                        }
                        
                        // Process the message only if it's valid
                        if (!message.empty()) {
                            handleMessage(message);
                        }
                        
                        // Continue receiving
                        if (client_connected_ && socket_.is_open()) {
                            startReceive();
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "Receive processing error: " << e.what() << std::endl;
                        client_connected_ = false;
                        asio::error_code ec;
                        socket_.close(ec);
                        startAccept();
                    }
                } else {
                    if (error == asio::error::eof || 
                        error == asio::error::connection_reset ||
                        error == asio::error::operation_aborted) {
                        std::cout << "Client disconnected" << std::endl;
                    } else {
                        std::cerr << "Receive error: " << error.message() << std::endl;
                    }
                    client_connected_ = false;
                    asio::error_code ec;
                    socket_.close(ec);
                    // Accept new connections
                    startAccept();
                }
            });
    }
    
    void handleMessage(const std::string& message) {
        try {
            // Early exit for empty messages
            if (message.empty()) {
                return;
            }
            
            // Safe trim - avoid iterator issues
            std::string trimmed;
            try {
                trimmed = message;
                
                // Find first non-whitespace
                size_t start = trimmed.find_first_not_of(" \t\r\n");
                if (start == std::string::npos || start >= trimmed.length()) {
                    return; // Empty or whitespace only
                }
                
                // Find last non-whitespace
                size_t end = trimmed.find_last_not_of(" \t\r\n");
                if (end == std::string::npos || end < start || end >= trimmed.length()) {
                    return; // Invalid range
                }
                
                // Safe substring extraction
                if (start == 0 && end == trimmed.length() - 1) {
                    // Already trimmed, no need to substr
                } else if (end >= start && (end - start + 1) <= trimmed.length()) {
                    trimmed = trimmed.substr(start, end - start + 1);
                } else {
                    return; // Invalid substring range
                }
            } catch (const std::exception& e) {
                std::cerr << "String trimming error: " << e.what() << std::endl;
                return;
            }
            
            if (trimmed.empty()) {
                return; // Ignore empty messages
            }
            
            json j = json::parse(trimmed);
            
            std::string type = j.value("type", "");
            
            if (type == "control") {
                // Control command
                double throttle = j.value("throttle", 0.0);
                double elevator = j.value("elevator", 0.0);
                double aileron = j.value("aileron", 0.0);
                double rudder = j.value("rudder", 0.0);
                
                if (on_control_command_) {
                    on_control_command_(throttle, elevator, aileron, rudder);
                }
                
                // Send acknowledgment
                json response;
                response["type"] = "ack";
                response["message"] = "Control command received";
                sendMessage(response);
                
            } else if (type == "query") {
                // Query command
                std::string command = j.value("command", "");
                
                if (command == "getState" && on_query_state_) {
                    json state = on_query_state_();
                    sendMessage(state);
                } else {
                    json error;
                    error["type"] = "error";
                    error["message"] = "Unknown query command";
                    sendMessage(error);
                }
                
            } else {
                json error;
                error["type"] = "error";
                error["message"] = "Unknown message type";
                sendMessage(error);
            }
            
        } catch (const json::exception& e) {
            std::cerr << "JSON parse error: " << e.what() << " (message: '" << message << "')" << std::endl;
            try {
                json error;
                error["type"] = "error";
                error["message"] = "Invalid JSON format";
                sendMessage(error);
            } catch (...) {
                // Ignore errors when sending error message
            }
        } catch (const std::exception& e) {
            std::cerr << "Message handling error: " << e.what() << std::endl;
        }
    }
    
    void sendWelcomeMessage() {
        json welcome;
        welcome["type"] = "welcome";
        welcome["message"] = "Connected to Flight Simulation Server";
        welcome["version"] = "1.0.0";
        sendMessage(welcome);
    }
    
    void sendMessage(const json& j) {
        if (!client_connected_ || !socket_.is_open()) return;
        
        try {
            // Create a shared_ptr to keep the message alive during async operation
            auto message = std::make_shared<std::string>(j.dump() + "\n");
            
            asio::async_write(socket_, asio::buffer(*message),
                [this, message](const asio::error_code& error, std::size_t /*bytes_transferred*/) {
                    // message is captured by value in shared_ptr, keeping it alive
                    if (error) {
                        if (error != asio::error::operation_aborted) {
                            std::cerr << "Send error: " << error.message() << std::endl;
                        }
                        client_connected_ = false;
                        asio::error_code ec;
                        socket_.close(ec);
                        startAccept();
                    }
                });
        } catch (const std::exception& e) {
            std::cerr << "Send message creation error: " << e.what() << std::endl;
            client_connected_ = false;
        }
    }

public:
    NetworkServer(asio::io_context& io_context, unsigned short port)
        : io_context_(io_context),
          acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
          socket_(io_context),
          client_connected_(false)
    {
        std::cout << "Network server listening on port " << port << std::endl;
        startAccept();
    }
    
    // Set callback for control commands
    void setControlCommandCallback(ControlCommandCallback callback) {
        on_control_command_ = callback;
    }
    
    // Set callback for state queries
    void setQueryStateCallback(QueryCommandCallback callback) {
        on_query_state_ = callback;
    }
    
    // Broadcast state update to connected client
    void broadcastState(const AircraftState& state, double time, double airDensity) {
        if (!client_connected_ || !socket_.is_open()) return;
        
        try {
            json j;
            j["type"] = "state";
            j["time"] = time;
            j["position"] = {
                {"x", state.position.x},
                {"y", state.position.y},
                {"z", state.position.z}
            };
            j["velocity"] = {
                {"x", state.velocity.x},
                {"y", state.velocity.y},
                {"z", state.velocity.z}
            };
            j["orientation"] = {
                {"roll", state.roll},
                {"pitch", state.pitch},
                {"yaw", state.yaw}
            };
            j["controls"] = {
                {"throttle", state.throttle},
                {"elevator", state.elevator},
                {"aileron", state.aileron},
                {"rudder", state.rudder}
            };
            j["rpm"] = state.currentRPM;
            j["altitude"] = -state.position.z;  // Z-down convention
            j["airspeed"] = state.velocity.magnitude();
            j["airDensity"] = airDensity;
            
            sendMessage(j);
        } catch (const std::exception& e) {
            std::cerr << "Broadcast state error: " << e.what() << std::endl;
        }
    }
    
    bool isClientConnected() const {
        return client_connected_;
    }
};

#endif // NETWORK_SERVER_H
