# test.ps1 - Test script for DigitRecognition project

Write-Host "Running tests for DigitRecognition project..." -ForegroundColor Green

# Check if build directory exists
if (-not (Test-Path "build")) {
    Write-Host "Build directory not found. Running build first..." -ForegroundColor Yellow
    & ".\scripts\build.ps1"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Build failed! Cannot run tests." -ForegroundColor Red
        exit 1
    }
}

# Change to build directory
Set-Location "build"

try {
    Write-Host "Running CTest..." -ForegroundColor Cyan
    Write-Host "=====================================`n" -ForegroundColor Yellow
    
    # Run CTest with verbose output and Debug configuration for MSVC builds
    ctest --output-on-failure --verbose -C Debug
    
    Write-Host "`n=====================================" -ForegroundColor Yellow
    if ($LASTEXITCODE -eq 0) {
        Write-Host "All tests passed! 🎉" -ForegroundColor Green
    } else {
        Write-Host "Some tests failed! ❌" -ForegroundColor Red
    }
} catch {
    Write-Host "Error running tests: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
} finally {
    # Return to project root
    Set-Location ..
}

Write-Host "Test script completed." -ForegroundColor Green