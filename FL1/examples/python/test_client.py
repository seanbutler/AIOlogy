#!/usr/bin/env python3
"""
Simple test client for Flight Simulation Server
Connects to server, sends control commands, and receives state updates
"""

import socket
import json
import time

def main():
    # Connect to server
    host = 'localhost'
    port = 9090
    
    print(f"Connecting to {host}:{port}...")
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    print("Connected!")
    
    # Receive welcome message
    data = sock.recv(4096).decode('utf-8')
    welcome = json.loads(data.strip())
    print(f"Welcome: {welcome}")
    
    try:
        # Send control command - increase throttle
        print("\nSending throttle command...")
        control_cmd = {
            "type": "control",
            "throttle": 0.7,
            "elevator": 0.0,
            "aileron": 0.0,
            "rudder": 0.0
        }
        sock.sendall((json.dumps(control_cmd) + "\n").encode('utf-8'))
        
        # Receive acknowledgment
        data = sock.recv(4096).decode('utf-8')
        ack = json.loads(data.strip())
        print(f"ACK: {ack}")
        
        # Wait for state updates
        print("\nReceiving state updates (press Ctrl+C to stop)...")
        count = 0
        while count < 10:  # Receive 10 updates
            data = sock.recv(4096).decode('utf-8')
            if data:
                state = json.loads(data.strip())
                if state.get('type') == 'state':
                    print(f"Time: {state['time']:.1f}s | "
                          f"Alt: {state['altitude']:.1f}m | "
                          f"Speed: {state['airspeed']:.1f}m/s | "
                          f"RPM: {state['rpm']:.0f}")
                    count += 1
        
        # Query current state
        print("\nQuerying current state...")
        query_cmd = {
            "type": "query",
            "command": "getState"
        }
        sock.sendall((json.dumps(query_cmd) + "\n").encode('utf-8'))
        
        # Receive query response
        data = sock.recv(4096).decode('utf-8')
        state = json.loads(data.strip())
        print(f"Current state: {json.dumps(state, indent=2)}")
        
    except KeyboardInterrupt:
        print("\nStopping...")
    finally:
        sock.close()
        print("Disconnected")

if __name__ == "__main__":
    main()
