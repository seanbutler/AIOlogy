"""
Main application entry point
"""

import torch
import matplotlib.pyplot as plt

from torchvision import transforms as tv_transforms
from torchvision import datasets as tv_datasets

#
# define a transform to convert images to tensors and normalise them
#

my_transform = tv_transforms.Compose([
    tv_transforms.ToTensor(),
    tv_transforms.Normalize((0.5,), (0.5,))
])

#
# Load the MNIST dataset
#

train_data = tv_datasets.MNIST(root='./data', 
                                train=True, 
                                download=True, 
                                transform=my_transform)

test_data = tv_datasets.MNIST(root='./data',
                               train=False,
                               download=True,
                               transform=my_transform)

train_loader = torch.utils.data.DataLoader(train_data, 
                                           batch_size=128, 
                                           shuffle=True)

test_loader = torch.utils.data.DataLoader(test_data,
                                          batch_size=1000,
                                          shuffle=False)


############################################################

class NeuralNetwork(torch.nn.Module):
    def __init__(self):
        super(NeuralNetwork, self).__init__()
        self.layer1 = torch.nn.Linear(28*28, 512)
        self.layer2 = torch.nn.Linear(512, 256)
        self.layer3 = torch.nn.Linear(256, 64)
        self.layer4 = torch.nn.Linear(64, 10)


    def train_forward(self, data):
        data = data.view(-1, 28*28)
        data = torch.nn.functional.relu(self.layer1(data))
        data = torch.nn.functional.relu(self.layer2(data))
        data = torch.nn.functional.relu(self.layer3(data))
        data = self.layer4(data)
        return data

############################################################

def calculate_accuracy(model, data_loader):
    """
    Calculate accuracy on a dataset
    
    Args:
        model: The neural network model
        data_loader: DataLoader for the dataset
        
    Returns:
        accuracy as a percentage
    """
    correct = 0
    total = 0
    
    # Don't compute gradients for validation
    with torch.no_grad():
        for images, labels in data_loader:
            outputs = model.train_forward(images)
            # Get the predicted class (highest score)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
    
    accuracy = 100 * correct / total
    return accuracy

############################################################

def plot_training_curves(loss_history, accuracy_history):
    """
    Plot training loss and accuracy over time
    
    Args:
        loss_history: List of loss values
        accuracy_history: List of accuracy values
    """
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 5))
    
    # Plot loss
    ax1.plot(loss_history, label='Training Loss', color='blue', linewidth=2)
    ax1.set_xlabel('Iteration (every 100 batches)', fontsize=12)
    ax1.set_ylabel('Loss', fontsize=12)
    ax1.set_title('Training Loss Over Time', fontsize=14)
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Plot accuracy
    epochs = range(1, len(accuracy_history) + 1)
    ax2.plot(epochs, accuracy_history, label='Test Accuracy', color='green', linewidth=2, marker='o')
    ax2.set_xlabel('Epoch', fontsize=12)
    ax2.set_ylabel('Accuracy (%)', fontsize=12)
    ax2.set_title('Test Accuracy Over Time', fontsize=14)
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('training_metrics.png')
    print("\nTraining metrics saved as 'training_metrics.png'")
    plt.show()

############################################################

def main():
    """Main function"""

    if torch.cuda.is_available():
        print(f"GPU: {torch.cuda.get_device_name(0)}")
        total_memory = torch.cuda.get_device_properties(0).total_memory / (1024**3)
        print(f"Total VRAM: {total_memory:.1f} GB")
    else:
        print("No GPU detected - running on CPU")


    my_Net = NeuralNetwork()
    for images, labels in train_loader:
        outputs = my_Net.train_forward(images)
        # print("Output:", outputs)
        break

    criterion = torch.nn.CrossEntropyLoss()
    optimizer = torch.optim.SGD(my_Net.parameters(), lr=0.001, momentum=0.9)

    print("my_Net", my_Net)

    # Lists to track metrics for plotting
    loss_history = []
    accuracy_history = []

    for epoch in range(30):
        running_loss = 0.0
        for i, data in enumerate(train_loader, 0):
            inputs, labels = data

            optimizer.zero_grad()

            outputs = my_Net.train_forward(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()

            running_loss += loss.item()
            if i % 100 == 99:
                avg_loss = running_loss / 100
                print(f"[{epoch + 1}, {i + 1}] loss: {avg_loss:.3f}")
                loss_history.append(avg_loss)  # Save for plotting
                running_loss = 0.0
        
        # Calculate accuracy at the end of each epoch
        accuracy = calculate_accuracy(my_Net, test_loader)
        accuracy_history.append(accuracy)
        print(f"Epoch {epoch + 1} - Test Accuracy: {accuracy:.2f}%")
    
    # Plot the training curves after training
    plot_training_curves(loss_history, accuracy_history)

############################################################

if __name__ == "__main__":
    main()
