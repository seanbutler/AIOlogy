# run.ps1 - Run script for DigitRecognition project

Write-Host "Running DigitRecognition project..." -ForegroundColor Green

# Check if build directory exists
if (-not (Test-Path "build")) {
    Write-Host "Build directory not found. Please run build.ps1 first." -ForegroundColor Red
    exit 1
}

# Check if executable exists
$exePath = "build\Debug\DigitRecognition.exe"
if (-not (Test-Path $exePath)) {
    # Try Release build
    $exePath = "build\Release\DigitRecognition.exe"
    if (-not (Test-Path $exePath)) {
        Write-Host "Executable not found. Please run build.ps1 first." -ForegroundColor Red
        exit 1
    }
}

try {
    Write-Host "Starting application..." -ForegroundColor Cyan
    Write-Host "=====================================`n" -ForegroundColor Yellow
    
    # Run the executable
    & $exePath
    
    Write-Host "`n=====================================" -ForegroundColor Yellow
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Application completed successfully!" -ForegroundColor Green
    } else {
        Write-Host "Application exited with error code: $LASTEXITCODE" -ForegroundColor Red
    }
} catch {
    Write-Host "Error running application: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}

Write-Host "Run script completed." -ForegroundColor Green