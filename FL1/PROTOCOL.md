# Flight Simulation Server - Network Protocol

## Overview

The Flight Simulation Server uses a **JSON-over-TCP** protocol for communication. Messages are newline-delimited JSON objects sent over a persistent TCP connection on port **9090** (default).

## Connection

```
Client connects to: localhost:9090 (or configured port)
Protocol: TCP
Format: JSON messages terminated with '\n'
Encoding: UTF-8
```

## Message Types

### Server → Client Messages

#### 1. Welcome Message
Sent immediately upon connection.

```json
{
  "type": "welcome",
  "message": "Connected to Flight Simulation Server",
  "version": "1.0.0"
}
```

#### 2. State Broadcast
Sent periodically (default: 10 Hz) to all connected clients.

```json
{
  "type": "state",
  "time": 12.5,
  "position": {
    "x": 125.3,
    "y": 0.0,
    "z": 45.2
  },
  "velocity": {
    "x": 30.5,
    "y": 0.0,
    "z": 5.2
  },
  "orientation": {
    "roll": 0.0,
    "pitch": 0.087,
    "yaw": 0.0
  },
  "controls": {
    "throttle": 0.7,
    "elevator": 0.1,
    "aileron": 0.0,
    "rudder": 0.0
  },
  "rpm": 2130.0,
  "altitude": -45.2,
  "airspeed": 31.2,
  "airDensity": 1.225
}
```

**Field Descriptions:**
- `time`: Simulation time in seconds
- `position`: Aircraft position in meters (world frame, Z-down convention)
  - `x`: East (+) / West (-)
  - `y`: North (+) / South (-)
  - `z`: Down (+) / Up (-) - **Note: Positive Z is downward**
- `velocity`: Aircraft velocity in m/s (world frame)
- `orientation`: Euler angles in radians
  - `roll`: Bank angle (positive = right wing down)
  - `pitch`: Nose angle (positive = nose up)
  - `yaw`: Heading angle (positive = clockwise from north)
- `controls`: Current control surface positions (range: -1.0 to +1.0, except throttle: 0.0 to 1.0)
  - `throttle`: Engine power (0.0 = idle, 1.0 = full power)
  - `elevator`: Pitch control (positive = nose up)
  - `aileron`: Roll control (positive = right roll)
  - `rudder`: Yaw control (positive = nose right)
- `rpm`: Current engine RPM
- `altitude`: Altitude above starting position in meters (negative = below start, positive = above start)
- `airspeed`: Total velocity magnitude in m/s
- `airDensity`: Air density in kg/m³

#### 3. Acknowledgment
Sent in response to successful control commands.

```json
{
  "type": "ack",
  "message": "Control command received"
}
```

#### 4. Error Message
Sent when a command fails or invalid data is received.

```json
{
  "type": "error",
  "message": "Invalid JSON format"
}
```

or

```json
{
  "type": "error",
  "message": "Unknown message type"
}
```

or

```json
{
  "type": "error",
  "message": "Unknown query command"
}
```

### Client → Server Messages

#### 1. Control Command
Update aircraft control surfaces.

```json
{
  "type": "control",
  "throttle": 0.8,
  "elevator": 0.1,
  "aileron": -0.05,
  "rudder": 0.0
}
```

**Field Descriptions:**
- `throttle`: Engine power setting
  - Range: `0.0` to `1.0`
  - `0.0` = idle (minimum RPM)
  - `1.0` = full power (maximum RPM)
  
- `elevator`: Pitch control
  - Range: `-1.0` to `+1.0`
  - `-1.0` = full nose down
  - `0.0` = neutral
  - `+1.0` = full nose up
  
- `aileron`: Roll control
  - Range: `-1.0` to `+1.0`
  - `-1.0` = full left roll (left wing down)
  - `0.0` = wings level
  - `+1.0` = full right roll (right wing down)
  
- `rudder`: Yaw control
  - Range: `-1.0` to `+1.0`
  - `-1.0` = nose left
  - `0.0` = neutral
  - `+1.0` = nose right

**Response:** Server sends an `ack` message, followed by the next state broadcast containing the updated control values.

**Notes:**
- All control values are clamped by the server to their valid ranges
- Values take effect immediately in the physics simulation
- RPM changes gradually based on configured spool rates (not instantaneous)

#### 2. State Query
Request the current aircraft state on-demand.

```json
{
  "type": "query",
  "command": "getState"
}
```

**Response:** Server sends a `state` message with current aircraft data.

**Available Commands:**
- `getState`: Returns current simulation state

## Communication Flow

### Typical Session

```
1. Client connects to server
   → Server sends: welcome message

2. Client sends: control command
   → Server sends: ack message
   → Server sends: state broadcasts (continuous, 10 Hz)

3. Client sends: query command
   → Server sends: state message (immediate)

4. Client disconnects
   → Server continues running, accepts new connections
```

### Example Python Client

```python
import socket
import json

# Connect
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('localhost', 9090))

# Receive welcome
data = sock.recv(4096).decode('utf-8')
welcome = json.loads(data.strip())
print(welcome)

# Send control command
control = {
    "type": "control",
    "throttle": 0.75,
    "elevator": 0.0,
    "aileron": 0.0,
    "rudder": 0.0
}
sock.sendall((json.dumps(control) + "\n").encode('utf-8'))

# Receive state updates
while True:
    data = sock.recv(4096).decode('utf-8')
    state = json.loads(data.strip())
    if state['type'] == 'state':
        print(f"Alt: {state['altitude']:.1f}m, Speed: {state['airspeed']:.1f}m/s")

sock.close()
```

## Coordinate Systems

### World Frame (Right-Handed, Z-Down)
- **X-axis**: East (positive) / West (negative)
- **Y-axis**: North (positive) / South (negative)  
- **Z-axis**: Down (positive) / Up (negative) - **Aerospace convention**

This means:
- `altitude = -position.z` (altitude is the negative of Z position)
- Aircraft starting at ground level has `position.z = 0`, `altitude = 0`
- Aircraft at 100m altitude has `position.z = -100`, `altitude = 100`
- Aircraft at -50m (underground) has `position.z = 50`, `altitude = -50`

### Body Frame
- **X-axis**: Forward (nose direction)
- **Y-axis**: Right wing
- **Z-axis**: Downward (perpendicular to wings)

## Physics Parameters

### Engine Dynamics
- **RPM Range**: Typically 800-2700 RPM
- **Spool Rate**: Configurable acceleration/deceleration
- **Power Curve**: Quadratic relationship (thrust ∝ RPM²)
- **Throttle Response**: Not instantaneous; RPM changes gradually

### Aerodynamics
- **Lift**: Depends on airspeed, wing area, angle of attack
- **Drag**: Parasitic + induced drag
- **Air Density**: Varies with altitude (ISA model)
- **Gravity**: Configurable per planet (Earth default: 9.81 m/s²)

### Update Rates
- **Physics Simulation**: Runs at timestep configured in `config.json` (default: 0.1s = 10 Hz)
- **State Broadcast**: 10 Hz (0.1 second interval)
- **Control Response**: Immediate (applied on next physics step)

## Error Handling

### Client Disconnection
- Server detects disconnect via TCP errors
- Server cleans up connection
- Server continues running and accepts new connections
- Simulation continues uninterrupted

### Invalid Messages
- Malformed JSON: Server sends error message, ignores command
- Unknown message type: Server sends error message
- Unknown query command: Server sends error message
- Empty/whitespace messages: Ignored silently

### Connection Limits
- **Current**: Single client at a time
- **Future**: Multi-client support planned (Phase 3)

## Configuration

Server startup:
```bash
FlightSimServer.exe [config.json] [port]
```

Default values:
- Config file: `config.json`
- Port: `9090`

## Protocol Version

**Current Version**: 1.0.0

Future protocol changes will increment the version number in the welcome message.

## See Also

- `examples/python/test_client.py` - Complete Python client example
- `examples/powershell/test_client.ps1` - PowerShell client example
- `ROADMAP.md` - Planned protocol enhancements
- `README.md` - General project documentation
