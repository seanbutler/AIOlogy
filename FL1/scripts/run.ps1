# Flight Simulation Run Script

param(
    [string]$ConfigFile = "config.json"
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Running Flight Simulation" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Get the script directory and project root
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectRoot = Split-Path -Parent $scriptDir

# Find the executable
$exePaths = @(
    "$projectRoot\build\apps\cli\Release\FlightSim.exe",
    "$projectRoot\build\apps\cli\Debug\FlightSim.exe"
)

$exeFound = $null
foreach ($path in $exePaths) {
    if (Test-Path $path) {
        $exeFound = $path
        break
    }
}

if ($exeFound) {
    Write-Host "Found executable: $exeFound" -ForegroundColor Green
    Write-Host "Configuration file: $ConfigFile" -ForegroundColor Green
    Write-Host ""
    
    # Run the simulation from project root (so relative paths work)
    Push-Location $projectRoot
    try {
        & $exeFound $ConfigFile
    } finally {
        Pop-Location
    }
    
    $exitCode = $LASTEXITCODE
    Write-Host ""
    
    if ($exitCode -eq 0) {
        Write-Host "========================================" -ForegroundColor Green
        Write-Host "  Simulation completed successfully" -ForegroundColor Green
        Write-Host "========================================" -ForegroundColor Green
    } else {
        Write-Host "========================================" -ForegroundColor Red
        Write-Host "  Simulation exited with code $exitCode" -ForegroundColor Red
        Write-Host "========================================" -ForegroundColor Red
    }
    
    exit $exitCode
    
} else {
    Write-Host "Error: Executable not found!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Searched locations:" -ForegroundColor Yellow
    foreach ($path in $exePaths) {
        Write-Host "  - $path" -ForegroundColor White
    }
    Write-Host ""
    Write-Host "Please build the project first using:" -ForegroundColor Yellow
    Write-Host "  .\scripts\build.ps1" -ForegroundColor White
    Write-Host ""
    exit 1
}
