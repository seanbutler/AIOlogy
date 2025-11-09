#ifndef AIRCRAFT_STATE_H
#define AIRCRAFT_STATE_H

#include "Vec3.h"
#include <cmath>

struct AircraftState {
    // Position in world space (meters)
    Vec3 position;
    
    // Orientation using Euler angles (radians)
    // Roll (φ), Pitch (θ), Yaw (ψ)
    double roll;   // Rotation around X-axis (body frame)
    double pitch;  // Rotation around Y-axis (body frame)
    double yaw;    // Rotation around Z-axis (body frame)
    
    // Linear velocity in world space (m/s)
    Vec3 velocity;
    
    // Angular velocity in body frame (rad/s)
    Vec3 angularVelocity;
    
    // Control inputs (normalized -1 to 1)
    double throttle;  // 0 to 1
    double elevator;  // -1 (nose down) to 1 (nose up)
    double aileron;   // -1 (roll left) to 1 (roll right)
    double rudder;    // -1 (left) to 1 (right)
    
    // Engine state
    double currentRPM; // Current engine RPM

    AircraftState() 
        : position(0, 0, 0),
          roll(0), pitch(0), yaw(0),
          velocity(0, 0, 0),
          angularVelocity(0, 0, 0),
          throttle(0),
          elevator(0),
          aileron(0),
          rudder(0),
          currentRPM(800.0)  // Start at idle RPM
    {}

    // Get forward direction vector in world space
    Vec3 getForwardVector() const {
        // Using aerospace convention: X-forward, Y-right, Z-down
        double cy = std::cos(yaw);
        double sy = std::sin(yaw);
        double cp = std::cos(pitch);
        double sp = std::sin(pitch);
        double cr = std::cos(roll);
        double sr = std::sin(roll);
        
        return Vec3(
            cy * cp,
            sy * cp,
            sp
        );
    }

    // Get right direction vector in world space
    Vec3 getRightVector() const {
        double cy = std::cos(yaw);
        double sy = std::sin(yaw);
        double cp = std::cos(pitch);
        double sp = std::sin(pitch);
        double cr = std::cos(roll);
        double sr = std::sin(roll);
        
        return Vec3(
            cy * sp * sr - sy * cr,
            sy * sp * sr + cy * cr,
            cp * sr
        );
    }

    // Get up direction vector in world space
    Vec3 getUpVector() const {
        double cy = std::cos(yaw);
        double sy = std::sin(yaw);
        double cp = std::cos(pitch);
        double sp = std::sin(pitch);
        double cr = std::cos(roll);
        double sr = std::sin(roll);
        
        return Vec3(
            cy * sp * cr + sy * sr,
            sy * sp * cr - cy * sr,
            cp * cr
        );
    }

    // Transform vector from world space to body space
    Vec3 worldToBody(const Vec3& worldVec) const {
        double cy = std::cos(yaw);
        double sy = std::sin(yaw);
        double cp = std::cos(pitch);
        double sp = std::sin(pitch);
        double cr = std::cos(roll);
        double sr = std::sin(roll);
        
        // Rotation matrix (world to body)
        return Vec3(
            cy * cp * worldVec.x + sy * cp * worldVec.y + sp * worldVec.z,
            (cy * sp * sr - sy * cr) * worldVec.x + (sy * sp * sr + cy * cr) * worldVec.y + cp * sr * worldVec.z,
            (cy * sp * cr + sy * sr) * worldVec.x + (sy * sp * cr - cy * sr) * worldVec.y + cp * cr * worldVec.z
        );
    }

    // Transform vector from body space to world space
    Vec3 bodyToWorld(const Vec3& bodyVec) const {
        double cy = std::cos(yaw);
        double sy = std::sin(yaw);
        double cp = std::cos(pitch);
        double sp = std::sin(pitch);
        double cr = std::cos(roll);
        double sr = std::sin(roll);
        
        // Rotation matrix (body to world) - transpose of world to body
        return Vec3(
            cy * cp * bodyVec.x + (cy * sp * sr - sy * cr) * bodyVec.y + (cy * sp * cr + sy * sr) * bodyVec.z,
            sy * cp * bodyVec.x + (sy * sp * sr + cy * cr) * bodyVec.y + (sy * sp * cr - cy * sr) * bodyVec.z,
            sp * bodyVec.x + cp * sr * bodyVec.y + cp * cr * bodyVec.z
        );
    }
};

#endif // AIRCRAFT_STATE_H
