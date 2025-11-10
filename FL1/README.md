# Flight Simulation Library

Modular C++ physics library for aircraft simulation with multiple applications.

## Overview

A high-fidelity, reusable physics engine for aircraft simulation featuring:

- **Realistic Aerodynamics**: Lift, drag, thrust with altitude-dependent air density
- **Engine Dynamics**: RPM-based power with spool-up/down lag
- **Planetary Environments**: Configurable gravity and atmospheres (Earth, Mars, custom)
- **6-DOF Motion**: Full 6 degrees of freedom physics
- **JSON Configuration**: All parameters loaded from JSON files
- **Modular Design**: Core library usable in CLI, GUI, network servers, tests

## Quick Start

### Build and Run

```powershell
# Build
.\scripts\build.ps1

# Run CLI app with default config
.\scripts\run.ps1

# Run with custom config
.\scripts\run.ps1 -ConfigFile config_mars.json

# Or run directly
.\build\apps\cli\Release\FlightSim.exe config.json
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
Project Structure:
├── lib/FlightSim/           → Core physics library (header-only)
│   ├── include/FlightSim/
│   │   ├── AircraftPhysics.h    → Physics engine
│   │   ├── AircraftSpec.h       → Aircraft properties
│   │   ├── AircraftState.h      → State representation
│   │   ├── Planet.h             → Environment model
│   │   ├── ConfigLoader.h       → JSON configuration
│   │   └── Vec3.h               → Vector math
│   └── CMakeLists.txt
├── apps/
│   └── cli/                 → Command-line application
│       ├── main.cpp         → CSV output simulation
│       └── CMakeLists.txt
├── scripts/                 → Build and run automation
└── config*.json             → Configuration files
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
