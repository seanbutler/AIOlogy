"""
Neural Network for Flight Simulation Control
"""

import torch
# import torch.nn as nn

class FlightControlNN(torch.nn.Module):
    """
    Neural network for learning flight control.
    
    Inputs (13):
        - position (x, y, z)
        - velocity (x, y, z)
        - orientation (roll, pitch, yaw)
        - rpm
        - altitude
        - airspeed
        - airDensity
    
    Outputs (4):
        - throttle (0-1)
        - elevator (-1 to +1)
        - aileron (-1 to +1)
        - rudder (-1 to +1)
    """
    
    def __init__(self, input_size=13, hidden_size=64, output_size=4):
        super(FlightControlNN, self).__init__()
        
        self.network = nn.Sequential(
            torch.nn.Linear(input_size, hidden_size),
            torch.nn.ReLU(),
            torch.nn.Linear(hidden_size, hidden_size),
            torch.nn.ReLU(),
            torch.nn.Linear(hidden_size, output_size),
            torch.nn.Tanh()  # Output in range [-1, 1]
        )
    
    def forward(self, x):
        """
        Forward pass through the network.
        
        Args:
            x: Input tensor of shape (batch_size, input_size)
        
        Returns:
            Output tensor of shape (batch_size, output_size)
        """
        output = self.network(x)
        
        # Adjust throttle to [0, 1] range (convert from tanh [-1,1] to [0,1])
        throttle = (output[:, 0:1] + 1.0) / 2.0
        controls = output[:, 1:4]  # elevator, aileron, rudder stay in [-1, 1]
        
        return torch.cat([throttle, controls], dim=1)
    
    def predict(self, state_dict):
        """
        Predict control outputs from a state dictionary.
        
        Args:
            state_dict: Dictionary containing flight state data
        
        Returns:
            Dictionary with control commands (throttle, elevator, aileron, rudder)
        """
        # Extract state features
        features = [
            state_dict['position']['x'],
            state_dict['position']['y'],
            state_dict['position']['z'],
            state_dict['velocity']['x'],
            state_dict['velocity']['y'],
            state_dict['velocity']['z'],
            state_dict['orientation']['roll'],
            state_dict['orientation']['pitch'],
            state_dict['orientation']['yaw'],
            state_dict['rpm'],
            state_dict['altitude'],
            state_dict['airspeed'],
            state_dict['airDensity']
        ]
        
        # Convert to tensor
        x = torch.tensor(features, dtype=torch.float32).unsqueeze(0)
        
        # Forward pass
        with torch.no_grad():
            output = self.forward(x)
        
        # Convert to control dictionary
        controls = {
            'throttle': float(output[0, 0]),
            'elevator': float(output[0, 1]),
            'aileron': float(output[0, 2]),
            'rudder': float(output[0, 3])
        }
        
        return controls



def create_control_network(hidden_size=64):
    """Factory function to create a flight control network."""
    return FlightControlNN(input_size=13, hidden_size=hidden_size, output_size=4)



if __name__ == "__main__":
    # Test the networks
    print("Testing FlightControlNN...")
    control_net = create_control_network()
    test_state = torch.randn(1, 13)
    controls = control_net(test_state)
    print(f"Input shape: {test_state.shape}")
    print(f"Output shape: {controls.shape}")
    print(f"Sample controls: {controls}")
    
    
    print("\nNetwork created successfully!")
