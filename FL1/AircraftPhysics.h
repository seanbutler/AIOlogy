#ifndef AIRCRAFT_PHYSICS_H
#define AIRCRAFT_PHYSICS_H

#include "Vec3.h"
#include "AircraftSpec.h"
#include "AircraftState.h"
#include <cmath>

class AircraftPhysics {
private:
    const AircraftSpec& spec;
    
    // Physical constants
    static constexpr double GRAVITY = 9.81;        // m/s^2
    static constexpr double AIR_DENSITY = 1.225;   // kg/m^3 at sea level

public:
    AircraftPhysics(const AircraftSpec& aircraftSpec) : spec(aircraftSpec) {}

    // Calculate air speed (magnitude of velocity)
    double getAirspeed(const AircraftState& state) const {
        return state.velocity.magnitude();
    }

    // Calculate angle of attack (radians)
    double getAngleOfAttack(const AircraftState& state) const {
        Vec3 velBody = state.worldToBody(state.velocity);
        if (velBody.x < 0.001) return 0.0; // Avoid division by zero
        return std::atan2(velBody.z, velBody.x);
    }

    // Calculate dynamic pressure: q = 0.5 * ρ * v^2
    double getDynamicPressure(const AircraftState& state) const {
        double airspeed = getAirspeed(state);
        return 0.5 * AIR_DENSITY * airspeed * airspeed;
    }

    // Calculate lift force in body frame
    Vec3 calculateLift(const AircraftState& state) const {
        double q = getDynamicPressure(state);
        double aoa = getAngleOfAttack(state);
        
        // Lift coefficient varies with angle of attack
        double CL = spec.liftCoeff * std::sin(2.0 * aoa);
        
        // Lift magnitude: L = q * S * CL
        double liftMag = q * spec.wingArea * CL;
        
        // Lift acts perpendicular to velocity, in the up direction (body frame)
        // Simplified: lift acts along negative Z-axis in body frame
        return Vec3(0, 0, -liftMag);
    }

    // Calculate drag force in body frame
    Vec3 calculateDrag(const AircraftState& state) const {
        double q = getDynamicPressure(state);
        double aoa = getAngleOfAttack(state);
        double CL = spec.liftCoeff * std::sin(2.0 * aoa);
        
        // Total drag: CD = CD0 + K*CL^2 (induced drag)
        double CD = spec.parasiteDrag + spec.inducedDragFactor * CL * CL;
        
        // Drag magnitude: D = q * S * CD
        double dragMag = q * spec.wingArea * CD;
        
        // Drag opposes velocity direction (in body frame)
        Vec3 velBody = state.worldToBody(state.velocity);
        Vec3 dragDir = velBody.normalized() * -1.0;
        
        return dragDir * dragMag;
    }

    // Calculate thrust from RPM using a power curve
    double calculateThrustFromRPM(double rpm) const {
        // Normalize RPM to 0-1 range
        double rpmNormalized = (rpm - spec.minRPM) / (spec.maxRPM - spec.minRPM);
        rpmNormalized = std::max(0.0, std::min(1.0, rpmNormalized));
        
        // Power curve: thrust increases with RPM^2 (more realistic than linear)
        // This simulates propeller efficiency and power characteristics
        double thrustFactor = rpmNormalized * rpmNormalized;
        
        return thrustFactor * spec.maxThrust;
    }
    
    // Calculate thrust force in body frame
    Vec3 calculateThrust(const AircraftState& state) const {
        double thrustMag = calculateThrustFromRPM(state.currentRPM);
        
        // Thrust acts along forward direction (X-axis in body frame)
        return Vec3(thrustMag * std::cos(spec.thrustAngle), 
                    0, 
                    thrustMag * std::sin(spec.thrustAngle));
    }

    // Calculate gravity force in world frame
    Vec3 calculateGravity() const {
        // Gravity acts downward in world frame (negative Z if Z is up)
        // Using aerospace convention where Z is down, so gravity is positive Z
        return Vec3(0, 0, spec.mass * GRAVITY);
    }

    // Calculate total force in world frame
    Vec3 calculateTotalForce(const AircraftState& state) const {
        // Calculate forces in body frame
        Vec3 liftBody = calculateLift(state);
        Vec3 dragBody = calculateDrag(state);
        Vec3 thrustBody = calculateThrust(state);
        
        // Transform to world frame
        Vec3 liftWorld = state.bodyToWorld(liftBody);
        Vec3 dragWorld = state.bodyToWorld(dragBody);
        Vec3 thrustWorld = state.bodyToWorld(thrustBody);
        
        // Gravity is already in world frame
        Vec3 gravityWorld = calculateGravity();
        
        // Sum all forces
        return thrustWorld + liftWorld + dragWorld + gravityWorld;
    }

    // Calculate moments (torques) in body frame
    Vec3 calculateMoments(const AircraftState& state) const {
        double q = getDynamicPressure(state);
        
        // Simplified moment model based on control inputs
        // Roll moment (L) from ailerons
        double rollMoment = q * spec.wingArea * spec.wingSpan * 
                           spec.aileronEffectiveness * state.aileron;
        
        // Pitch moment (M) from elevator
        double pitchMoment = q * spec.wingArea * spec.wingSpan * 
                            spec.elevatorEffectiveness * state.elevator;
        
        // Yaw moment (N) from rudder
        double yawMoment = q * spec.wingArea * spec.wingSpan * 
                          spec.rudderEffectiveness * state.rudder;
        
        // Add damping based on angular velocities
        rollMoment -= 0.5 * state.angularVelocity.x * spec.inertiaX;
        pitchMoment -= 0.5 * state.angularVelocity.y * spec.inertiaY;
        yawMoment -= 0.5 * state.angularVelocity.z * spec.inertiaZ;
        
        return Vec3(rollMoment, pitchMoment, yawMoment);
    }

    // Update RPM based on throttle input with lag
    void updateRPM(AircraftState& state, double dt) const {
        // Target RPM based on throttle position
        double targetRPM = spec.minRPM + state.throttle * (spec.maxRPM - spec.minRPM);
        
        // Calculate RPM change with spool rates
        double rpmDiff = targetRPM - state.currentRPM;
        
        if (rpmDiff > 0) {
            // Spooling up
            double rpmIncrease = spec.rpmSpoolUpRate * dt;
            state.currentRPM += std::min(rpmDiff, rpmIncrease);
        } else if (rpmDiff < 0) {
            // Spooling down
            double rpmDecrease = spec.rpmSpoolDownRate * dt;
            state.currentRPM += std::max(rpmDiff, -rpmDecrease);
        }
        
        // Clamp to valid range
        state.currentRPM = std::max(spec.minRPM, std::min(spec.maxRPM, state.currentRPM));
    }
    
    // Integrate state forward by timestep using RK4 (Runge-Kutta 4th order)
    AircraftState step(const AircraftState& state, double dt) const {
        // For simplicity, using Euler integration here
        // For better accuracy, implement RK4
        AircraftState newState = state;
        
        // Update engine RPM with lag
        updateRPM(newState, dt);
        
        // Calculate accelerations
        Vec3 totalForce = calculateTotalForce(state);
        Vec3 linearAccel = totalForce / spec.mass;
        
        Vec3 moments = calculateMoments(state);
        Vec3 angularAccel(
            moments.x / spec.inertiaX,
            moments.y / spec.inertiaY,
            moments.z / spec.inertiaZ
        );
        
        // Update velocities
        newState.velocity = state.velocity + linearAccel * dt;
        newState.angularVelocity = state.angularVelocity + angularAccel * dt;
        
        // Update position
        newState.position = state.position + state.velocity * dt;
        
        // Update orientation (Euler angles)
        // Note: This is simplified and can have gimbal lock issues
        // For production, use quaternions
        newState.roll = state.roll + state.angularVelocity.x * dt;
        newState.pitch = state.pitch + state.angularVelocity.y * dt;
        newState.yaw = state.yaw + state.angularVelocity.z * dt;
        
        // Normalize angles to [-π, π]
        newState.roll = normalizeAngle(newState.roll);
        newState.pitch = normalizeAngle(newState.pitch);
        newState.yaw = normalizeAngle(newState.yaw);
        
        return newState;
    }

    // Simulate forward for multiple steps
    AircraftState simulate(const AircraftState& initialState, double duration, double dt) const {
        AircraftState state = initialState;
        int steps = static_cast<int>(duration / dt);
        
        for (int i = 0; i < steps; ++i) {
            state = step(state, dt);
        }
        
        return state;
    }

private:
    // Normalize angle to [-π, π]
    double normalizeAngle(double angle) const {
        const double PI = 3.14159265358979323846;
        while (angle > PI) angle -= 2.0 * PI;
        while (angle < -PI) angle += 2.0 * PI;
        return angle;
    }
};

#endif // AIRCRAFT_PHYSICS_H
