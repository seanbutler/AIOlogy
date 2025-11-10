# Aircraft Physics Simulation

Real-time aircraft flight dynamics simulation with network interface for external control and monitoring.

## Overview

A high-fidelity physics engine for aircraft simulation featuring:

- **Realistic Aerodynamics**: Lift, drag, thrust with altitude-dependent air density
- **Engine Dynamics**: RPM-based power with spool-up/down lag
- **Planetary Environments**: Configurable gravity and atmospheres (Earth, Mars, custom)
- **6-DOF Motion**: Full 6 degrees of freedom physics
- **JSON Configuration**: All parameters loaded from JSON files
- **CSV Output**: Time-series data export for analysis

## Quick Start

### Build and Run

```powershell
# Build
.\scripts\build.ps1

# Run with default config
.\scripts\run.ps1

# Run with custom config
.\build\Debug\aircraft_sim.exe config_mars.json
```

### Configuration

All simulation parameters are in JSON:

```json
{
  "aircraftSpec": { "mass": 1200, "wingArea": 16, ... },
  "aircraftState": { "position": {...}, "controls": {...} },
  "planet": { "gravity": 9.81, "atmosphere": {...} },
  "simulationParameters": { "dt": 0.1, "totalTime": 100 }
}
```

See `config.json` (Earth) or `config_mars.json` for examples.

## Architecture

```
Core Components:
├── AircraftPhysics.h    → Physics engine (forces, integration)
├── AircraftSpec.h       → Aircraft properties (mass, thrust, etc)
├── AircraftState.h      → Current state (position, velocity, controls)
├── Planet.h             → Environment (gravity, atmosphere)
├── ConfigLoader.h       → JSON configuration loading
└── Vec3.h               → 3D vector math

Build System:
├── CMakeLists.txt       → CMake configuration with FetchContent
└── scripts/             → Build and run scripts
```

## Current Features

✅ Altitude-dependent air density (ISA model)  
✅ Engine RPM dynamics with power curves  
✅ Configurable planetary environments  
✅ JSON-based configuration  
✅ CSV data export  
✅ Multiple coordinate frame transformations  

## Roadmap

See [ROADMAP.md](ROADMAP.md) for detailed development plan.

### Next: Network Interface (In Progress)
Transform sim into network server with JSON protocol over TCP sockets:
- Control aircraft via network commands
- Real-time state streaming
- Multi-client support
- Python/C++ client libraries

### Future Goals
- 3D visualization client (OpenGL + ImGui)
- AI/autopilot integration (Python RL)
- Multi-aircraft simulation
- Advanced physics (quaternions, RK4, weather)
- Flight scenarios and data analysis tools

## Dependencies

Automatically fetched by CMake:
- [nlohmann/json](https://github.com/nlohmann/json) - JSON parsing
- (Soon) [Asio](https://github.com/chriskohlhoff/asio) - Networking

## Physics Model

**Aerodynamics:**
- Dynamic pressure: `q = 0.5 × ρ(h) × v²`
- Lift: `L = q × S × CL(α)`
- Drag: `D = q × S × (CD₀ + K×CL²)`

**Atmosphere:**
- ISA model with temperature lapse rate
- Exponential decay for thin atmospheres
- Configurable per planet

**Engine:**
- Thrust = f(RPM) with quadratic power curve
- RPM lag with configurable spool rates

**Coordinate System:**
- Z-down aerospace convention
- Body/world frame transformations
- Euler angles (quaternions planned)

## Contributing

This is an active development project. See [ROADMAP.md](ROADMAP.md) for priorities.

## License

Free to use and modify.
