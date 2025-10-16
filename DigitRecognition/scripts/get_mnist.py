# save_mnist_torch.py
from torchvision import datasets
import os

root = "./data/mnist_data"
out_dir = "./data/mnist_images"   # output folder for PNGs
os.makedirs(out_dir, exist_ok=True)

# download train and test
for split in [("train", True), ("test", False)]:
    name, train_flag = split
    ds = datasets.MNIST(root=root, download=True, train=train_flag)
    split_dir = os.path.join(out_dir, name)
    for image, label in ds:
        label_dir = os.path.join(split_dir, str(label))
        os.makedirs(label_dir, exist_ok=True)
    # iterate with index to create unique filenames
    for idx in range(len(ds)):
        img, lbl = ds[idx]
        # img is already a PIL Image from MNIST dataset
        fname = os.path.join(out_dir, name, str(lbl), f"{lbl}_{idx:05d}.png")
        img.save(fname)
print("Saved MNIST images to", out_dir)
