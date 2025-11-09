# Aircraft Physics Simulation

A C++ physics-based simulation for airplane flight dynamics.

## Overview

This simulation takes aircraft specifications and current state (position, orientation, velocity) as inputs and calculates the aircraft's future position using realistic physics models including:

- **Aerodynamic Forces**: Lift and drag based on wing area, speed, and angle of attack
- **Thrust**: Engine thrust with configurable power
- **Gravity**: Standard gravitational force
- **Control Surfaces**: Elevator, aileron, and rudder inputs
- **6-DOF Dynamics**: Full 6 degrees of freedom (position + orientation)

## Project Structure

- `Vec3.h` - 3D vector math utilities (add, subtract, dot, cross products, etc.)
- `AircraftSpec.h` - Aircraft specification structure (mass, wing area, drag coefficients, thrust, inertia)
- `AircraftState.h` - Current state of the aircraft (position, orientation, velocity, control inputs)
- `AircraftPhysics.h` - Physics engine that calculates forces and integrates state forward in time
- `main.cpp` - Example simulation demonstrating a takeoff scenario
- `CMakeLists.txt` - CMake build configuration

## Building

### Using CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Using g++ directly (Windows PowerShell)

```powershell
g++ -std=c++17 -o aircraft_sim.exe main.cpp
```

### Using MSVC directly (Windows)

```powershell
cl /EHsc /std:c++17 main.cpp /Fe:aircraft_sim.exe
```

## Running

After building, run the executable:

```bash
./aircraft_sim      # Linux/Mac
aircraft_sim.exe    # Windows
```

## Usage Example

```cpp
#include "AircraftPhysics.h"

// Define aircraft specifications
AircraftSpec spec;
spec.mass = 1200.0;           // kg
spec.wingArea = 16.0;         // m^2
spec.maxThrust = 6000.0;      // N

// Create physics engine
AircraftPhysics physics(spec);

// Set initial state
AircraftState state;
state.position = Vec3(0, 0, 0);
state.velocity = Vec3(20, 0, 0);  // 20 m/s forward
state.throttle = 0.7;              // 70% throttle

// Simulate forward in time
double dt = 0.01;  // 10ms timestep
AircraftState futureState = physics.step(state, dt);

// Or simulate multiple steps at once
AircraftState predictedState = physics.simulate(state, 5.0, dt);
```

## Physics Models

### Forces Calculated
- **Lift**: `L = q × S × CL` where q is dynamic pressure, S is wing area, CL is lift coefficient
- **Drag**: `D = q × S × (CD₀ + K×CL²)` including parasitic and induced drag
- **Thrust**: Configurable engine thrust along body X-axis
- **Gravity**: Standard gravitational acceleration

### Coordinate System
- **World Frame**: Standard aerospace convention (Z-down, X-forward, Y-right)
- **Body Frame**: Aircraft-relative coordinates
- **Euler Angles**: Roll, pitch, yaw for orientation

### Integration
Currently uses Euler integration with configurable timestep. Can be upgraded to RK4 for better accuracy.

## Customization

Modify `AircraftSpec` parameters to simulate different aircraft:
- Small general aviation aircraft (default)
- Fighters (high thrust-to-weight ratio)
- Commercial jets (high mass, large wing area)
- Aerobatic aircraft (high control surface effectiveness)

## Future Enhancements
- Quaternion-based orientation (avoid gimbal lock)
- RK4 integration for better accuracy
- Wind and atmospheric models
- More sophisticated aerodynamic models
- Ground effect during takeoff/landing
- Engine dynamics and fuel consumption

## License

Free to use and modify.
