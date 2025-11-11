# Flight Simulation Server Run Script

param(
    [string]$ConfigFile = "config.json",
    [int]$Port = 9090
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Running Flight Simulation Server" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Get the script directory and project root
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectRoot = Split-Path -Parent $scriptDir

# Find the executable
$exePaths = @(
    "$projectRoot\build\apps\server\Release\FlightSimServer.exe",
    "$projectRoot\build\apps\server\Debug\FlightSimServer.exe"
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
    Write-Host "Network port: $Port" -ForegroundColor Green
    Write-Host ""
    
    # Run the server from project root (so relative paths work)
    Push-Location $projectRoot
    try {
        & $exeFound $ConfigFile $Port
    } finally {
        Pop-Location
    }
    
    $exitCode = $LASTEXITCODE
    Write-Host ""
    
    if ($exitCode -eq 0) {
        Write-Host "========================================" -ForegroundColor Green
        Write-Host "  Server stopped successfully" -ForegroundColor Green
        Write-Host "========================================" -ForegroundColor Green
    } else {
        Write-Host "========================================" -ForegroundColor Red
        Write-Host "  Server exited with code $exitCode" -ForegroundColor Red
        Write-Host "========================================" -ForegroundColor Red
    }
    
    exit $exitCode
    
} else {
    Write-Host "Error: Server executable not found!" -ForegroundColor Red
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
