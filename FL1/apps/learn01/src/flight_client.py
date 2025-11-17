"""
Flight Simulation Client for network communication
"""

import socket
import json
import threading
import time
from typing import Optional, Callable, Dict


class FlightSimClient:
    """
    Client for communicating with the Flight Simulation Server.
    
    Handles TCP connection, sending control commands, and receiving state updates.
    """
    
    def __init__(self, host='localhost', port=9090):
        """
        Initialize the client.
        
        Args:
            host: Server hostname or IP address
            port: Server port number
        """
        self.host = host
        self.port = port
        self.socket: Optional[socket.socket] = None
        self.connected = False
        self.running = False
        
        # Latest state from server
        self.latest_state: Optional[Dict] = None
        self.state_lock = threading.Lock()
        
        # Callback for state updates
        self.state_callback: Optional[Callable[[Dict], None]] = None
        
        # Receive thread
        self.receive_thread: Optional[threading.Thread] = None
    
    def connect(self) -> bool:
        """
        Connect to the flight simulation server.
        
        Returns:
            True if connection successful, False otherwise
        """
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            self.connected = True
            
            # Receive welcome message
            data = self.socket.recv(4096).decode('utf-8')
            welcome = json.loads(data.strip())
            print(f"Connected: {welcome.get('message', 'Unknown')}")
            print(f"Version: {welcome.get('version', 'Unknown')}")
            
            # Start receive thread
            self.running = True
            self.receive_thread = threading.Thread(target=self._receive_loop, daemon=True)
            self.receive_thread.start()
            
            return True
            
        except ConnectionRefusedError:
            print(f"Error: Connection refused to {self.host}:{self.port}")
            print("Is the server running?")
            self.connected = False
            return False
            
        except Exception as e:
            print(f"Error connecting: {e}")
            self.connected = False
            return False
    
    def disconnect(self):
        """Disconnect from the server."""
        self.running = False
        self.connected = False
        
        if self.receive_thread and self.receive_thread.is_alive():
            self.receive_thread.join(timeout=2.0)
        
        if self.socket:
            try:
                self.socket.close()
            except:
                pass
            self.socket = None
        
        print("Disconnected from server")
    
    def _receive_loop(self):
        """Background thread for receiving messages from server."""
        buffer = ""
        
        while self.running and self.connected:
            try:
                data = self.socket.recv(4096).decode('utf-8')
                if not data:
                    print("Server closed connection")
                    self.connected = False
                    break
                
                buffer += data
                
                # Process complete messages (newline-delimited)
                while '\n' in buffer:
                    line, buffer = buffer.split('\n', 1)
                    line = line.strip()
                    
                    if line:
                        try:
                            message = json.loads(line)
                            self._handle_message(message)
                        except json.JSONDecodeError as e:
                            print(f"JSON decode error: {e}")
                
            except socket.timeout:
                continue
            except Exception as e:
                if self.running:
                    print(f"Error receiving data: {e}")
                self.connected = False
                break
    
    def _handle_message(self, message: Dict):
        """
        Handle incoming message from server.
        
        Args:
            message: Parsed JSON message
        """
        msg_type = message.get('type')
        
        if msg_type == 'state':
            # Update latest state
            with self.state_lock:
                self.latest_state = message
            
            # Call callback if registered
            if self.state_callback:
                try:
                    self.state_callback(message)
                except Exception as e:
                    print(f"Error in state callback: {e}")
        
        elif msg_type == 'ack':
            pass  # Control command acknowledged
        
        elif msg_type == 'error':
            print(f"Server error: {message.get('message', 'Unknown error')}")
    
    def get_state(self) -> Optional[Dict]:
        """
        Get the latest state from the server.
        
        Returns:
            Dictionary containing latest state, or None if no state received yet
        """
        with self.state_lock:
            return self.latest_state.copy() if self.latest_state else None
    
    def send_controls(self, throttle: float, elevator: float, aileron: float, rudder: float) -> bool:
        """
        Send control commands to the server.
        
        Args:
            throttle: Engine power (0.0 to 1.0)
            elevator: Pitch control (-1.0 to +1.0)
            aileron: Roll control (-1.0 to +1.0)
            rudder: Yaw control (-1.0 to +1.0)
        
        Returns:
            True if sent successfully, False otherwise
        """
        if not self.connected:
            print("Error: Not connected to server")
            return False
        
        # Clamp values to valid ranges
        throttle = max(0.0, min(1.0, throttle))
        elevator = max(-1.0, min(1.0, elevator))
        aileron = max(-1.0, min(1.0, aileron))
        rudder = max(-1.0, min(1.0, rudder))
        
        control_msg = {
            "type": "control",
            "throttle": throttle,
            "elevator": elevator,
            "aileron": aileron,
            "rudder": rudder
        }
        
        try:
            message = json.dumps(control_msg) + "\n"
            self.socket.sendall(message.encode('utf-8'))
            return True
        except Exception as e:
            print(f"Error sending controls: {e}")
            self.connected = False
            return False
    
    def query_state(self) -> bool:
        """
        Request current state from server (on-demand).
        
        Returns:
            True if query sent successfully, False otherwise
        """
        if not self.connected:
            print("Error: Not connected to server")
            return False
        
        query_msg = {
            "type": "query",
            "command": "getState"
        }
        
        try:
            message = json.dumps(query_msg) + "\n"
            self.socket.sendall(message.encode('utf-8'))
            return True
        except Exception as e:
            print(f"Error sending query: {e}")
            self.connected = False
            return False
    
    def set_state_callback(self, callback: Callable[[Dict], None]):
        """
        Register a callback function to be called on each state update.
        
        Args:
            callback: Function that takes a state dictionary as argument
        """
        self.state_callback = callback
    
    def is_connected(self) -> bool:
        """Check if client is connected to server."""
        return self.connected
    
    def wait_for_state(self, timeout: float = 5.0) -> Optional[Dict]:
        """
        Wait for the first state message from server.
        
        Args:
            timeout: Maximum time to wait in seconds
        
        Returns:
            First state received, or None if timeout
        """
        start_time = time.time()
        
        while time.time() - start_time < timeout:
            state = self.get_state()
            if state:
                return state
            time.sleep(0.01)
        
        return None


def main():
    """Test the client."""
    print("Flight Simulation Client Test")
    print("=" * 50)
    
    # Create client
    client = FlightSimClient()
    
    # Connect
    if not client.connect():
        return 1
    
    # Wait for first state
    print("\nWaiting for state...")
    state = client.wait_for_state()
    
    if state:
        print(f"Altitude: {state['altitude']:.1f}m")
        print(f"Airspeed: {state['airspeed']:.1f}m/s")
        print(f"RPM: {state['rpm']:.0f}")
    
    # Send some control commands
    print("\nSending control commands...")
    client.send_controls(throttle=0.75, elevator=0.1, aileron=0.0, rudder=0.0)
    
    # Monitor state for a few seconds
    print("\nMonitoring state for 3 seconds...")
    start_time = time.time()
    while time.time() - start_time < 3.0:
        state = client.get_state()
        if state:
            print(f"Alt: {state['altitude']:6.1f}m  Speed: {state['airspeed']:5.1f}m/s  "
                  f"Throttle: {state['controls']['throttle']:.2f}")
        time.sleep(0.5)
    
    # Disconnect
    print("\nDisconnecting...")
    client.disconnect()
    
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main())
