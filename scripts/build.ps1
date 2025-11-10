# Flight Simulation Build Script for Windows PowerShell

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Flight Simulation Build Script" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check if CMake is available
$cmakeAvailable = Get-Command cmake -ErrorAction SilentlyContinue

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
                $exe = Get-ChildItem -Recurse -Filter "FlightSim.exe" | Select-Object -First 1
                if ($exe) {
                    Write-Host "  $($exe.FullName)" -ForegroundColor White
                    Write-Host ""
                    Write-Host "Run with:" -ForegroundColor Cyan
                    Write-Host "  .\scripts\run.ps1" -ForegroundColor White
                    Write-Host "or:" -ForegroundColor Cyan
                    Write-Host "  .\$($exe.FullName -replace [regex]::Escape($PWD.Path + '\'), '') [config.json]" -ForegroundColor White
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
    
} else {
    Write-Host "Error: CMake is required for this project!" -ForegroundColor Red
    Write-Host ""
    Write-Host "This project uses a library structure that requires CMake." -ForegroundColor Yellow
    Write-Host "Please install CMake from https://cmake.org/download/" -ForegroundColor White
    exit 1
}

Write-Host ""
