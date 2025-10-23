#!/usr/bin/env python3
"""
Plot neural network training loss curves from CSV file.
Usage: python plot_loss.py [loss_file.csv]
"""

import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

def plot_loss(csv_file="training_loss.csv"):
    """Plot loss curves from training CSV file."""
    
    if not os.path.exists(csv_file):
        print(f"Error: File '{csv_file}' not found.")
        print("Make sure to run the C++ application first to generate the loss data.")
        return
    
    try:
        # Read CSV data
        df = pd.read_csv(csv_file)
        print(f"Loaded {len(df)} epochs of training data from {csv_file}")
        
        # Create subplots
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))
        
        # Plot total loss
        ax1.plot(df['epoch'], df['total_loss'], 'b-', linewidth=2, marker='o')
        ax1.set_xlabel('Epoch')
        ax1.set_ylabel('Total Loss')
        ax1.set_title('Total Loss per Epoch')
        ax1.grid(True, alpha=0.3)
        
        # Plot average loss
        ax2.plot(df['epoch'], df['avg_loss'], 'r-', linewidth=2, marker='s')
        ax2.set_xlabel('Epoch')
        ax2.set_ylabel('Average Loss per Sample')
        ax2.set_title('Average Loss per Epoch')
        ax2.grid(True, alpha=0.3)
        
        # Add stats
        min_total_loss = df['total_loss'].min()
        min_avg_loss = df['avg_loss'].min()
        final_total_loss = df['total_loss'].iloc[-1]
        final_avg_loss = df['avg_loss'].iloc[-1]
        
        ax1.text(0.02, 0.98, f'Min: {min_total_loss:.4f}\nFinal: {final_total_loss:.4f}', 
                transform=ax1.transAxes, verticalalignment='top', 
                bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))
        
        ax2.text(0.02, 0.98, f'Min: {min_avg_loss:.6f}\nFinal: {final_avg_loss:.6f}', 
                transform=ax2.transAxes, verticalalignment='top',
                bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))
        
        plt.tight_layout()
        
        # Save plot
        plot_file = csv_file.replace('.csv', '_plot.png')
        plt.savefig(plot_file, dpi=300, bbox_inches='tight')
        print(f"Plot saved as: {plot_file}")
        
        # Show plot
        plt.show()
        
        # Print summary statistics
        print("\n=== TRAINING SUMMARY ===")
        print(f"Epochs trained: {len(df)}")
        print(f"Samples per epoch: {df['samples'].iloc[0]}")
        print(f"Initial total loss: {df['total_loss'].iloc[0]:.4f}")
        print(f"Final total loss: {final_total_loss:.4f}")
        print(f"Loss reduction: {((df['total_loss'].iloc[0] - final_total_loss) / df['total_loss'].iloc[0] * 100):.1f}%")
        print(f"Initial avg loss: {df['avg_loss'].iloc[0]:.6f}")
        print(f"Final avg loss: {final_avg_loss:.6f}")
        print(f"Minimum avg loss: {min_avg_loss:.6f} (epoch {df.loc[df['avg_loss'].idxmin(), 'epoch']})")
        
    except Exception as e:
        print(f"Error reading CSV file: {e}")
        print("Make sure the CSV file has the expected format: epoch,total_loss,avg_loss,samples")

if __name__ == "__main__":
    csv_file = sys.argv[1] if len(sys.argv) > 1 else "training_loss.csv"
    plot_loss(csv_file)