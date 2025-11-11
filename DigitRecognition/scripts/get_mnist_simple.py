#!/usr/bin/env python3
"""
Simple MNIST dataset downloader without torchvision dependency.
Downloads the original MNIST dataset files and converts them to individual PNG images.
"""

import os
import gzip
import struct
import urllib.request
from PIL import Image
import numpy as np

def download_file(url, filename):
    """Download a file if it doesn't exist."""
    if not os.path.exists(filename):
        print(f"Downloading {filename}...")
        urllib.request.urlretrieve(url, filename)
        print(f"Downloaded {filename}")
    else:
        print(f"{filename} already exists, skipping download")

def read_mnist_images(filename):
    """Read MNIST image file."""
    with gzip.open(filename, 'rb') as f:
        magic, num, rows, cols = struct.unpack('>IIII', f.read(16))
        images = np.frombuffer(f.read(), dtype=np.uint8).reshape(num, rows, cols)
    return images

def read_mnist_labels(filename):
    """Read MNIST label file."""
    with gzip.open(filename, 'rb') as f:
        magic, num = struct.unpack('>II', f.read(8))
        labels = np.frombuffer(f.read(), dtype=np.uint8)
    return labels

def main():
    # Create directories
    data_dir = "./data/mnist_data"
    out_dir = "./data/mnist_images"
    os.makedirs(data_dir, exist_ok=True)
    os.makedirs(out_dir, exist_ok=True)
    
    # MNIST dataset URLs (using mirror since original might be down)
    base_url = "https://ossci-datasets.s3.amazonaws.com/mnist/"
    files = {
        "train-images-idx3-ubyte.gz": "train_images",
        "train-labels-idx1-ubyte.gz": "train_labels", 
        "t10k-images-idx3-ubyte.gz": "test_images",
        "t10k-labels-idx1-ubyte.gz": "test_labels"
    }
    
    # Download files
    for filename in files.keys():
        url = base_url + filename
        filepath = os.path.join(data_dir, filename)
        download_file(url, filepath)
    
    # Process train and test sets
    for split in ["train", "test"]:
        print(f"\nProcessing {split} set...")
        
        if split == "train":
            images_file = os.path.join(data_dir, "train-images-idx3-ubyte.gz")
            labels_file = os.path.join(data_dir, "train-labels-idx1-ubyte.gz")
        else:
            images_file = os.path.join(data_dir, "t10k-images-idx3-ubyte.gz")
            labels_file = os.path.join(data_dir, "t10k-labels-idx1-ubyte.gz")
        
        # Read data
        images = read_mnist_images(images_file)
        labels = read_mnist_labels(labels_file)
        
        # Create output directory
        split_dir = os.path.join(out_dir, split)
        os.makedirs(split_dir, exist_ok=True)
        
        # Save images
        print(f"Saving {len(images)} images...")
        for idx, (img, lbl) in enumerate(zip(images, labels)):
            # Convert to PIL Image and save
            pil_img = Image.fromarray(img, mode='L')
            filename = f"{lbl}_{idx:05d}.png"
            filepath = os.path.join(split_dir, filename)
            pil_img.save(filepath)
            
            if (idx + 1) % 5000 == 0:
                print(f"  Saved {idx + 1}/{len(images)} images")
    
    print(f"\nMNIST dataset saved to {out_dir}")
    print("Directory structure:")
    print(f"  {out_dir}/train/ - Training images")
    print(f"  {out_dir}/test/ - Test images")

if __name__ == "__main__":
    main()
