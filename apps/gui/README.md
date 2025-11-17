# GUI

A Python tkinter GUI application for controlling the Flight Simulation Server.

## Overview

This GUI application provides a visual interface for:
- Controlling aircraft flight surfaces (throttle, aileron, elevator)
- Sending control commands to the flight simulation server
- Querying aircraft state
- Viewing telemetry and log output

## Setup

1. Create and activate the virtual environment:
   ```powershell
   python -m venv venv
   .\venv\Scripts\Activate.ps1
   ```

2. Install dependencies:
   ```powershell
   pip install -r requirements.txt
   ```

## Usage

1. Start the flight simulation server:
   ```powershell
   # From FL1 root directory
   .\build\Debug\apps\server\FlightSimServer.exe
   ```

2. Run the GUI application:
   ```powershell
   python src\main.py
   ```

3. Use the sliders to control the aircraft:
   - **Throttle**: 0-100% engine power
   - **Aileron**: -100 to +100 (roll control)
   - **Elevator**: -100 to +100 (pitch control)

4. Use the buttons:
   - **Send**: Sends current slider values to server
   - **Query**: Requests current aircraft state from server
   - **Get**: (Reserved for future use)

## Project Structure

```
gui/
├── src/           # Source code
│   ├── __init__.py
│   └── main.py    # Main tkinter GUI application
├── tests/         # Test files
│   ├── __init__.py
│   └── test_main.py
├── venv/          # Virtual environment (created after setup)
├── requirements.txt
├── .gitignore
└── README.md
```

## Features

- Real-time control sliders for flight surfaces
- TCP connection to flight simulation server (localhost:9090)
- JSON message protocol for commands
- Text output area for logging and feedback
- Error handling for connection issues

## Current Limitations

- Only sends commands, doesn't receive state updates
- Creates new connection for each button press (not persistent)
- Missing rudder control
- No real-time telemetry display

## Development

To deactivate the virtual environment:
```powershell
deactivate
```

## See Also

- `../../PROTOCOL.md` - Network protocol documentation
- `../../examples/python/test_client.py` - Reference client implementation
- `../learn01/` - Machine learning application for flight control
