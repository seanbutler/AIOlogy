# build.ps1 - Build script for DigitRecognition project

Write-Host "Building DigitRecognition project..." -ForegroundColor Green

# Create build directory if it doesn't exist
if (-not (Test-Path "build")) {
    Write-Host "Creating build directory..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Name "build"
}

# Change to build directory
Set-Location "build"

try {
    # Generate build files
    Write-Host "Generating build files..." -ForegroundColor Cyan
    cmake ..
    
    if ($LASTEXITCODE -eq 0) {
        # Build the project
        Write-Host "Building project..." -ForegroundColor Cyan
        cmake --build .
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "Build completed successfully!" -ForegroundColor Green
        } else {
            Write-Host "Build failed!" -ForegroundColor Red
            exit 1
        }
    } else {
        Write-Host "CMake configuration failed!" -ForegroundColor Red
        exit 1
    }
} finally {
    # Return to project root
    Set-Location ..
}

Write-Host "Build script completed." -ForegroundColor Green