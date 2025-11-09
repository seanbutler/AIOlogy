#ifndef AIRCRAFT_SPEC_H
#define AIRCRAFT_SPEC_H

struct AircraftSpec {
    // Basic properties
    double mass;              // kg
    double wingArea;          // m^2
    double wingSpan;          // m
    
    // Aerodynamic coefficients
    double liftCoeff;         // CL (lift coefficient)
    double dragCoeff;         // CD (drag coefficient)
    double sideForceCoeff;    // Side force coefficient
    
    // Drag components
    double parasiteDrag;      // CD0 (zero-lift drag)
    double inducedDragFactor; // K for induced drag (CD = CD0 + K*CL^2)
    
    // Engine properties
    double maxThrust;         // Newtons
    double thrustAngle;       // radians (angle relative to aircraft body)
    double minRPM;            // Minimum RPM (idle)
    double maxRPM;            // Maximum RPM (redline)
    double rpmSpoolUpRate;    // RPM increase rate per second
    double rpmSpoolDownRate;  // RPM decrease rate per second
    
    // Moments of inertia (kg*m^2)
    double inertiaX;          // Roll axis
    double inertiaY;          // Pitch axis
    double inertiaZ;          // Yaw axis
    
    // Control surface effectiveness
    double elevatorEffectiveness;
    double aileronEffectiveness;
    double rudderEffectiveness;

    // Constructor with default values (roughly based on a small aircraft)
    AircraftSpec() 
        : mass(1000.0),
          wingArea(16.0),
          wingSpan(10.0),
          liftCoeff(0.5),
          dragCoeff(0.05),
          sideForceCoeff(0.0),
          parasiteDrag(0.025),
          inducedDragFactor(0.05),
          maxThrust(5000.0),
          thrustAngle(0.0),
          minRPM(800.0),
          maxRPM(2700.0),
          rpmSpoolUpRate(500.0),
          rpmSpoolDownRate(300.0),
          inertiaX(500.0),
          inertiaY(800.0),
          inertiaZ(1200.0),
          elevatorEffectiveness(1.0),
          aileronEffectiveness(1.0),
          rudderEffectiveness(1.0)
    {}
};

#endif // AIRCRAFT_SPEC_H
