# Aircraft Simulation Build Script for Windows PowerShell

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Aircraft Simulation Build Script" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check if CMake is available
$cmakeAvailable = Get-Command cmake -ErrorAction SilentlyContinue
$gppAvailable = Get-Command g++ -ErrorAction SilentlyContinue
$clAvailable = Get-Command cl -ErrorAction SilentlyContinue

if ($cmakeAvailable) {
    Write-Host "Building with CMake..." -ForegroundColor Green
    Write-Host ""
    
    # Create build directory
    if (-not (Test-Path "build")) {
        New-Item -ItemType Directory -Path "build" | Out-Null
        Write-Host "Created build directory" -ForegroundColor Yellow
    }
    
    # Navigate to build directory
    Push-Location build
    
    try {
        # Configure
        Write-Host "Configuring project..." -ForegroundColor Yellow
        cmake ..
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host ""
            Write-Host "Building project..." -ForegroundColor Yellow
            cmake --build .
            
            if ($LASTEXITCODE -eq 0) {
                Write-Host ""
                Write-Host "========================================" -ForegroundColor Green
                Write-Host "  Build Successful!" -ForegroundColor Green
                Write-Host "========================================" -ForegroundColor Green
                Write-Host ""
                Write-Host "Executable location:" -ForegroundColor Cyan
                
                # Find the executable
                $exe = Get-ChildItem -Recurse -Filter "aircraft_sim.exe" | Select-Object -First 1
                if ($exe) {
                    Write-Host "  $($exe.FullName)" -ForegroundColor White
                    Write-Host ""
                    Write-Host "Run with:" -ForegroundColor Cyan
                    Write-Host "  cd build" -ForegroundColor White
                    Write-Host "  .\$($exe.FullName -replace [regex]::Escape($PWD.Path + '\'), '')" -ForegroundColor White
                }
            } else {
                Write-Host ""
                Write-Host "Build failed!" -ForegroundColor Red
                exit 1
            }
        } else {
            Write-Host ""
            Write-Host "CMake configuration failed!" -ForegroundColor Red
            exit 1
        }
    } finally {
        Pop-Location
    }
    
} elseif ($gppAvailable) {
    Write-Host "CMake not found. Building with g++..." -ForegroundColor Yellow
    Write-Host ""
    
    g++ -std=c++17 -Wall -O2 -o aircraft_sim.exe main.cpp
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "========================================" -ForegroundColor Green
        Write-Host "  Build Successful!" -ForegroundColor Green
        Write-Host "========================================" -ForegroundColor Green
        Write-Host ""
        Write-Host "Run with: .\aircraft_sim.exe" -ForegroundColor Cyan
    } else {
        Write-Host ""
        Write-Host "Build failed!" -ForegroundColor Red
        exit 1
    }
    
} elseif ($clAvailable) {
    Write-Host "CMake and g++ not found. Building with MSVC (cl)..." -ForegroundColor Yellow
    Write-Host ""
    
    cl /EHsc /std:c++17 /O2 main.cpp /Fe:aircraft_sim.exe
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "========================================" -ForegroundColor Green
        Write-Host "  Build Successful!" -ForegroundColor Green
        Write-Host "========================================" -ForegroundColor Green
        Write-Host ""
        Write-Host "Run with: .\aircraft_sim.exe" -ForegroundColor Cyan
        
        # Clean up MSVC intermediate files
        Remove-Item *.obj -ErrorAction SilentlyContinue
    } else {
        Write-Host ""
        Write-Host "Build failed!" -ForegroundColor Red
        exit 1
    }
    
} else {
    Write-Host "Error: No suitable compiler found!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please install one of the following:" -ForegroundColor Yellow
    Write-Host "  - CMake (recommended)" -ForegroundColor White
    Write-Host "  - MinGW-w64 (g++)" -ForegroundColor White
    Write-Host "  - Visual Studio Build Tools (cl)" -ForegroundColor White
    exit 1
}

Write-Host ""
