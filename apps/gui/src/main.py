"""
Main application entry point
"""

import tkinter as tk
from tkinter import ttk
import socket
import json


class MainWindow:
    """Main application window"""
    
    def __init__(self, root):
        self.root = root
        self.root.title("AIOlogy Pilot")
        self.root.geometry("800x600")
        
        # Create main frame
        main_frame = ttk.Frame(root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # Configure grid weight
        root.columnconfigure(0, weight=1)
        root.rowconfigure(0, weight=1)
        
        # Add a label
        label = ttk.Label(main_frame, text="Welcome to AIOlogy Pilot!", 
                         font=("Arial", 16))
        label.grid(row=0, column=0, pady=20)
        
        # Add sliders frame
        sliders_frame = ttk.LabelFrame(main_frame, text="Controls", padding="10")
        sliders_frame.grid(row=1, column=0, pady=10, sticky=(tk.W, tk.E))
        
        # Throttle slider
        ttk.Label(sliders_frame, text="Throttle:").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.throttle_var = tk.DoubleVar(value=0)
        self.throttle_slider = ttk.Scale(sliders_frame, from_=0, to=100, 
                                        orient=tk.HORIZONTAL, variable=self.throttle_var,
                                        command=self.on_throttle_change, length=300)
        self.throttle_slider.grid(row=0, column=1, padx=10, pady=5)
        self.throttle_label = ttk.Label(sliders_frame, text="0.0")
        self.throttle_label.grid(row=0, column=2, padx=5)
        
        # Aileron slider
        ttk.Label(sliders_frame, text="Aileron:").grid(row=1, column=0, sticky=tk.W, pady=5)
        self.aileron_var = tk.DoubleVar(value=0)
        self.aileron_slider = ttk.Scale(sliders_frame, from_=-100, to=100, 
                                       orient=tk.HORIZONTAL, variable=self.aileron_var,
                                       command=self.on_aileron_change, length=300)
        self.aileron_slider.grid(row=1, column=1, padx=10, pady=5)
        self.aileron_label = ttk.Label(sliders_frame, text="0.0")
        self.aileron_label.grid(row=1, column=2, padx=5)
        
        # Elevator slider
        ttk.Label(sliders_frame, text="Elevator:").grid(row=2, column=0, sticky=tk.W, pady=5)
        self.elevator_var = tk.DoubleVar(value=0)
        self.elevator_slider = ttk.Scale(sliders_frame, from_=-100, to=100, 
                                        orient=tk.HORIZONTAL, variable=self.elevator_var,
                                        command=self.on_elevator_change, length=300)
        self.elevator_slider.grid(row=2, column=1, padx=10, pady=5)
        self.elevator_label = ttk.Label(sliders_frame, text="0.0")
        self.elevator_label.grid(row=2, column=2, padx=5)
        
        # Add buttons frame
        buttons_frame = ttk.Frame(main_frame)
        buttons_frame.grid(row=2, column=0, pady=10)
        
        send_button = ttk.Button(buttons_frame, text="Send", 
                                command=self.on_send_click)
        send_button.grid(row=0, column=0, padx=5)
        
        get_button = ttk.Button(buttons_frame, text="Get", 
                               command=self.on_get_click)
        get_button.grid(row=0, column=1, padx=5)
        
        query_button = ttk.Button(buttons_frame, text="Query", 
                                 command=self.on_query_click)
        query_button.grid(row=0, column=2, padx=5)
        
        # Add a text widget
        self.text = tk.Text(main_frame, height=10, width=60)
        self.text.grid(row=3, column=0, pady=10)
        self.text.insert("1.0", "Application ready.\n")
    
    def on_throttle_change(self, value):
        """Handle throttle slider change"""
        val = float(value) / 100.0  # Convert 0-100 to 0.0-1.0
        self.throttle_label.config(text=f"{val:.2f}")
        self.text.insert(tk.END, f"Throttle: {val:.2f}\n")
        self.text.see(tk.END)
    
    def on_aileron_change(self, value):
        """Handle aileron slider change"""
        val = float(value) / 100.0  # Convert -100-100 to -1.0-1.0
        self.aileron_label.config(text=f"{val:.2f}")
        self.text.insert(tk.END, f"Aileron: {val:.2f}\n")
        self.text.see(tk.END)
    
    def on_elevator_change(self, value):
        """Handle elevator slider change"""
        val = float(value) / 100.0  # Convert -100-100 to -1.0-1.0
        self.elevator_label.config(text=f"{val:.2f}")
        self.text.insert(tk.END, f"Elevator: {val:.2f}\n")
        self.text.see(tk.END)
    
    def on_send_click(self):
        """Handle Send button click"""
        try:
            # Collect current slider values
            data = {
                "throttle": float(self.throttle_var.get()) / 100.0,
                "aileron": float(self.aileron_var.get()) / 100.0,
                "elevator": float(self.elevator_var.get()) / 100.0
            }
            
            # Convert to JSON
            json_data = json.dumps(data)
            
            # Connect to localhost:9090
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.settimeout(2)  # 2 second timeout
                s.connect(('localhost', 9090))
                s.sendall(json_data.encode('utf-8'))
                
            self.text.insert(tk.END, f"Sent: {json_data}\n")
            self.text.see(tk.END)
            
        except ConnectionRefusedError:
            self.text.insert(tk.END, "Error: Connection refused. Is server running on localhost:9090?\n")
            self.text.see(tk.END)
        except socket.timeout:
            self.text.insert(tk.END, "Error: Connection timeout\n")
            self.text.see(tk.END)
        except Exception as e:
            self.text.insert(tk.END, f"Error: {str(e)}\n")
            self.text.see(tk.END)
    
    def on_get_click(self):
        """Handle Get button click"""
        self.text.insert(tk.END, "Get button clicked!\n")
        self.text.see(tk.END)
    
    def on_query_click(self):
        """Handle Query button click"""
        try:
            query_cmd = {
                "type": "query",
                "command": "getState"
            }
            
            # Convert to JSON
            json_data = json.dumps(query_cmd)
            
            # Connect to localhost:9090
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.settimeout(2)  # 2 second timeout
                s.connect(('localhost', 9090))
                s.sendall(json_data.encode('utf-8'))
                
            self.text.insert(tk.END, f"Query sent: {json_data}\n")
            self.text.see(tk.END)
            
        except ConnectionRefusedError:
            self.text.insert(tk.END, "Error: Connection refused. Is server running on localhost:9090?\n")
            self.text.see(tk.END)
        except socket.timeout:
            self.text.insert(tk.END, "Error: Connection timeout\n")
            self.text.see(tk.END)
        except Exception as e:
            self.text.insert(tk.END, f"Error: {str(e)}\n")
            self.text.see(tk.END)


def main():
    """Main function"""
    root = tk.Tk()
    app = MainWindow(root)
    root.mainloop()


if __name__ == "__main__":
    main()
