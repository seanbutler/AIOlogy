# Learn01

A Python application for machine learning experiments with the flight sim

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

Run the application:
```powershell
python src\main.py
```

Run tests:
```powershell
python -m pytest tests\
```

## Project Structure

```
learn01/
├── src/           # Source code
│   ├── __init__.py
│   └── main.py
├── tests/         # Test files
│   ├── __init__.py
│   └── test_main.py
├── venv/          # Virtual environment (created after setup)
├── requirements.txt
├── .gitignore
└── README.md
```

## Development

To deactivate the virtual environment:
```powershell
deactivate
```
