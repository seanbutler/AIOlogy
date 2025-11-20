# AIOlogy

AI and machine learning learning repository - building algorithms from first principles.

## Projects

### 1. DigitRecognition
![Status](https://img.shields.io/badge/status-stable-green) ![C++](https://img.shields.io/badge/C++-23-blue)

From-scratch C++ neural network for MNIST handwritten digit recognition. Custom layers, activations (sigmoid, ReLU), JSON configuration, comprehensive testing.

**Quick Start:**
```powershell
cd DigitRecognition
.\scripts\build.ps1 && python .\scripts\get_mnist.py && .\scripts\run.ps1
```

**Tech:** C++23, CMake, nlohmann/json, SDL2  
📁 [Details →](DigitRecognition/)

---

### 2. FL1 - Aircraft Physics Simulation
![Status](https://img.shields.io/badge/status-active-blue)

Real-time 6-DOF flight dynamics with altitude-dependent aerodynamics, engine RPM modeling, and configurable planetary environments (Earth, Mars, custom). Network interface in progress for remote control.

**Quick Start:**
```powershell
cd FL1
.\scripts\build.ps1 && .\scripts\run.ps1
```

**Tech:** C++, CMake, nlohmann/json, TCP networking  
� [Details →](FL1/)

---

### 3. MNIST_Classifier
![Status](https://img.shields.io/badge/status-stable-green) ![Python](https://img.shields.io/badge/Python-3.8+-blue)

PyTorch-based neural network training framework for MNIST with experiment management, comprehensive visualization dashboards, and study-based batch experiments.

**Quick Start:**
```powershell
cd MNIST_Classifier
python -m venv venv && venv\Scripts\activate && pip install -r requirements.txt
python main.py
```

**Tech:** Python, PyTorch, Matplotlib, NumPy  
📁 [Details →](MNIST_Classifier/)

---

### 4. Exploring Linear Regression
![Status](https://img.shields.io/badge/status-in_progress-yellow)

Linear regression from first principles - gradient descent, cost functions, feature scaling.

**Tech:** Python  
📁 [Details →](Exploring%20Linear%20Regression/)

---

## Build System

**Prerequisites:** CMake 3.16+, C++17/23 compiler, PowerShell (Windows), Python 3.7+

**Build:**
```powershell
.\build.bat           # Root level
# Or per-project:
cd ProjectName && .\scripts\build.ps1
```

**CMake Options:** `BUILD_TESTS`, `BUILD_EXAMPLES`, `USE_OPENCV`, `USE_EIGEN`

## Philosophy

Learning by building from first principles - no black-box libraries, educational clarity over optimization, real-world data, comprehensive testing.

---

**Owner:** [seanbutler](https://github.com/seanbutler) | **Repo:** [AIOlogy](https://github.com/seanbutler/AIOlogy)