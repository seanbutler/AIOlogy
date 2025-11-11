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
                Write-Host "Executables built:" -ForegroundColor Cyan
                
                # Find CLI executable
                $cliExe = Get-ChildItem -Recurse -Filter "FlightSim.exe" -Path "apps\cli" -ErrorAction SilentlyContinue | Select-Object -First 1
                if ($cliExe) {
                    Write-Host "  CLI: $($cliExe.FullName)" -ForegroundColor White
                }
                
                # Find Server executable
                $serverExe = Get-ChildItem -Recurse -Filter "FlightSimServer.exe" -Path "apps\server" -ErrorAction SilentlyContinue | Select-Object -First 1
                if ($serverExe) {
                    Write-Host "  Server: $($serverExe.FullName)" -ForegroundColor White
                }
                
                Write-Host ""
                Write-Host "Run CLI with:" -ForegroundColor Cyan
                Write-Host "  .\scripts\run.ps1" -ForegroundColor White
                Write-Host ""
                Write-Host "Run Server with:" -ForegroundColor Cyan
                Write-Host "  .\scripts\run_server.ps1" -ForegroundColor White
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
