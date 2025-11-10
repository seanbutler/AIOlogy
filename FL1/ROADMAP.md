# Aircraft Physics Simulation - Development Roadmap

## Project Overview
A real-time aircraft physics simulation engine that can be controlled via network socket interface, allowing external applications to interact with the simulation through JSON messages.

---

## Current Next Goal: Project Refactoring

### Objective
Separate the physics simulation library from application code for better modularity and reusability.

### Why Refactor First?
- Clean separation of concerns before adding networking
- Physics library can be reused in multiple applications (CLI, network server, GUI, tests)
- Easier to test and maintain
- Standard C++ library structure

### New Project Structure
```
FL1/
├── CMakeLists.txt                  # Root CMake (builds all)
├── lib/
│   └── FlightSim/                  # Physics library
│       ├── CMakeLists.txt          # Library-specific CMake
│       ├── include/
│       │   └── FlightSim/
│       │       ├── Vec3.h
│       │       ├── AircraftSpec.h
│       │       ├── AircraftState.h
│       │       ├── AircraftPhysics.h
│       │       ├── Planet.h
│       │       └── ConfigLoader.h
│       └── src/                    # Implementation files (if needed)
├── apps/
│   └── cli/                        # Command-line application
│       ├── CMakeLists.txt
│       └── main.cpp                # Uses FlightSim library
├── scripts/                        # Build/run scripts
└── configs/                        # JSON configuration files
```

### Implementation Tasks

#### Refactoring Checklist
- [ ] Create `lib/FlightSim/` directory structure
- [ ] Move headers to `lib/FlightSim/include/FlightSim/`
- [ ] Create library CMakeLists.txt with proper include paths
- [ ] Move current main.cpp to `apps/cli/`
- [ ] Update root CMakeLists.txt to build library first
- [ ] Update app CMakeLists.txt to link against library
- [ ] Test build system works
- [ ] Update build scripts to work with new structure

#### Benefits
✅ Reusable physics library  
✅ Multiple applications can share same core  
✅ Easier unit testing  
✅ Professional project structure  
✅ Ready for network server as separate app  

---

## Goal: Network Interface

### Objective
Transform the sim into a continuous process with socket-based IPC (Inter-Process Communication).

### Architecture
```
┌─────────────────┐           JSON over TCP           ┌─────────────────┐
│                 │◄─────────────────────────────────►│                 │
│  Client Apps    │   Commands (throttle, etc)        │  Sim Server     │
│  (GUI, AI, etc) │   State (pos, vel, orientation)   │  (Physics Loop) │
│                 │                                   │                 │
└─────────────────┘                                   └─────────────────┘
```

### Technical Requirements

1. **Network Library Selection**
   - Library: **Asio** (standalone, header-only version)
   - Rationale:
     * Part of Boost, but available as standalone (no Boost dependency)
     * Cross-platform (Windows, Linux, macOS)
     * Well-documented, widely used
     * Modern C++ (async I/O)
     * Used in many professional projects
   - Alternative: cpp-httplib (simpler but HTTP-based)
   - GitHub: https://github.com/chriskohlhoff/asio

2. **Communication Protocol**
   - Protocol: JSON over TCP sockets
   - Port: Configurable (default: 9090)
   - Message format:
     ```json
     // Client → Server (Commands)
     {
       "type": "control",
       "throttle": 0.7,
       "elevator": 0.0,
       "aileron": 0.0,
       "rudder": 0.0
     }
     
     // Server → Client (State updates)
     {
       "type": "state",
       "time": 10.5,
       "position": {"x": 100, "y": 0, "z": -500},
       "velocity": {"x": 50, "y": 0, "z": -10},
       "orientation": {"roll": 0, "pitch": 5, "yaw": 0},
       "rpm": 2100,
       "altitude": 500,
       "airspeed": 52.5,
       "airDensity": 1.15
     }
     
     // Client → Server (Queries)
     {
       "type": "query",
       "command": "getState"
     }
     
     // Server → Client (Configuration)
     {
       "type": "config",
       "command": "setTimestep",
       "dt": 0.01
     }
     ```

3. **Server Architecture**
   - Main loop: Physics simulation (independent of network)
   - Network thread: Handle client connections
   - Update rate: Configurable (default: 10 Hz for network, variable for physics)
   - Multi-client: Support multiple observers
   - Thread-safe: Mutex for state access

### Implementation Tasks

#### Phase 0: Library Refactoring (Priority: CRITICAL - DO FIRST)
See "Current Next Goal: Project Refactoring" above.

#### Phase 1: Basic Network Server (Priority: HIGH)
- [ ] Add Asio to CMakeLists.txt via FetchContent
- [ ] Create `NetworkServer.h` class
- [ ] Implement TCP server listening on configurable port
- [ ] Accept single client connection
- [ ] Receive JSON commands, parse with nlohmann/json
- [ ] Send JSON state updates at fixed rate
- [ ] Test with telnet/netcat

#### Phase 2: Simulation Loop Refactor (Priority: HIGH)
- [ ] Separate physics loop from main()
- [ ] Create `SimulationEngine.h` class
- [ ] Implement continuous simulation loop
- [ ] Add thread-safe state access (mutex)
- [ ] Network commands update control inputs
- [ ] State broadcast at configurable rate

#### Phase 3: Multi-Client Support (Priority: MEDIUM)
- [ ] Support multiple simultaneous connections
- [ ] Broadcast state to all connected clients
- [ ] Each client can send control commands
- [ ] Client priority system (primary vs. observers)

#### Phase 4: Advanced Features (Priority: LOW)
- [ ] Pause/resume simulation
- [ ] Reset simulation
- [ ] Hot-reload configuration
- [ ] Record/playback capability
- [ ] Performance metrics

---

## Future Goals

### Goal 2: Visualization Client
**Objective:** Create a real-time 3D visualization client

**Tech Stack:**
- C++ with OpenGL or Vulkan
- Dear ImGui for UI
- Connects to sim via socket

**Features:**
- 3D aircraft model
- Terrain/skybox
- Instrument panel overlay
- Control inputs (keyboard/joystick)

---

### Goal 3: AI/Autopilot Integration
**Objective:** ML-based autonomous flight control

**Approach:**
- Python client using socket interface
- Reinforcement learning (stable-baselines3)
- Train agents for:
  * Takeoff/landing
  * Navigation
  * Aerobatics
  * Emergency procedures

---

### Goal 4: Multi-Aircraft Simulation
**Objective:** Support multiple aircraft in same airspace

**Features:**
- Each aircraft has unique ID
- Collision detection
- Wake turbulence effects
- Formation flying

---

### Goal 5: Advanced Physics
**Objective:** More realistic flight model

**Enhancements:**
- [ ] Quaternion-based orientation (avoid gimbal lock)
- [ ] RK4 integration (better accuracy)
- [ ] Ground effect
- [ ] Wind/weather simulation
- [ ] Engine temperature/fuel consumption
- [ ] Structural stress limits
- [ ] Control surface aerodynamics
- [ ] Stall modeling

---

### Goal 6: Scenario System
**Objective:** Pre-defined flight scenarios

**Examples:**
- Takeoff from specific runway
- Emergency landing
- Navigation challenges
- Weather events
- Combat maneuvers (for games)

---

### Goal 7: Data Logging & Analysis
**Objective:** Flight data recorder and analysis tools

**Features:**
- CSV/HDF5 output
- Flight path replay
- Performance analysis
- Python analysis scripts

---

## Software Engineering Best Practices

### Build System
- [x] CMake with FetchContent for dependencies
- [x] Build scripts (PowerShell)
- [ ] Cross-platform support (Linux, macOS)
- [ ] Unit tests (Google Test)
- [ ] CI/CD pipeline (GitHub Actions)

### Code Quality
- [ ] Code formatting (clang-format)
- [ ] Static analysis (clang-tidy)
- [ ] Memory safety (valgrind/sanitizers)
- [ ] Documentation (Doxygen)
- [ ] Performance profiling

### Project Structure (After Refactoring)
```
FL1/
├── lib/
│   └── FlightSim/              # Core physics library
│       ├── CMakeLists.txt
│       ├── include/FlightSim/  # Public headers
│       └── src/                # Implementation (if needed)
├── apps/
│   ├── cli/                    # Command-line app
│   │   ├── CMakeLists.txt
│   │   └── main.cpp
│   └── server/                 # Network server (future)
│       ├── CMakeLists.txt
│       ├── main.cpp
│       └── NetworkServer.h
├── tests/                      # Unit tests
├── scripts/                    # Build/run scripts
├── configs/                    # JSON configurations
├── docs/                       # Documentation
└── examples/                   # Example clients
    ├── python/                 # Python client examples
    └── cpp/                    # C++ client examples
```

### Dependencies Management
- [x] nlohmann/json (JSON parsing)
- [ ] Asio (networking)
- [ ] Google Test (testing)
- [ ] spdlog (logging)

---

## Timeline Estimate

### Sprint 0 (2-3 days): Library Refactoring
- Restructure project directories
- Create library CMakeLists
- Update build system
- Verify everything still works

### Sprint 1 (1-2 weeks): Network Foundation
- Integrate Asio
- Basic TCP server
- JSON command/state protocol
- Single client support

### Sprint 2 (1 week): Simulation Loop
- Refactor to continuous simulation
- Thread-safe state management
- Network integration

### Sprint 3 (1 week): Testing & Polish
- Multi-client support
- Error handling
- Simple test client (Python)

### Total: 3-4 weeks for networked simulation

---

## Success Metrics

1. **Performance**
   - Physics: 1000+ Hz update rate
   - Network: 10-100 Hz state broadcast
   - Latency: < 10ms from command to effect

2. **Reliability**
   - 24+ hours continuous operation
   - Graceful client disconnect handling
   - No memory leaks

3. **Usability**
   - Simple Python client in < 50 lines
   - Clear JSON protocol documentation
   - Example code for common tasks

---

## Getting Started (Next Steps)

1. Review and approve this plan
2. Set up branch for network feature
3. Integrate Asio library
4. Implement basic TCP server
5. Create simple test client

---

**Last Updated:** November 10, 2025
**Status:** Planning Phase
