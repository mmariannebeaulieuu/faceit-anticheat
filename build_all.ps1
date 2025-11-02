#!/usr/bin/env pwsh
# Build script for Anti-Cheat project (PowerShell)
# This script builds both the C++ client and the Python launcher executable

$ErrorActionPreference = "Stop"

Write-Host "================================================================" -ForegroundColor Cyan
Write-Host " Building Anti-Cheat Project" -ForegroundColor Cyan
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host ""

# Check if we're in the right directory
if (-not (Test-Path "CMakeLists.txt")) {
    Write-Host "[!] Error: CMakeLists.txt not found. Please run this from the project root." -ForegroundColor Red
    exit 1
}

# Step 1: Build the C++ Anti-Cheat Client
Write-Host "[1/3] Building C++ Anti-Cheat Client..." -ForegroundColor Yellow
Write-Host ""

if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}

Push-Location build

try {
    Write-Host "[*] Running CMake configuration..." -ForegroundColor Gray
    & cmake -G "Visual Studio 17 2022" -A x64 ..
    if ($LASTEXITCODE -ne 0) {
        # Try with Visual Studio 16 2019
        Write-Host "[*] Trying Visual Studio 16 2019..." -ForegroundColor Gray
        & cmake -G "Visual Studio 16 2019" -A x64 ..
        if ($LASTEXITCODE -ne 0) {
            # Try with Ninja or default generator
            Write-Host "[*] Trying default generator..." -ForegroundColor Gray
            & cmake ..
            if ($LASTEXITCODE -ne 0) {
                throw "CMake configuration failed"
            }
        }
    }
    
    Write-Host "[*] Building..." -ForegroundColor Gray
    & cmake --build . --config Release
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed"
    }
    
    Write-Host ""
    Write-Host "[+] C++ client built successfully" -ForegroundColor Green
    Write-Host ""
}
catch {
    Write-Host "[!] Error: $_" -ForegroundColor Red
    Pop-Location
    exit 1
}
finally {
    Pop-Location
}

# Step 2: Copy built files to distribution directory
Write-Host "[2/3] Preparing distribution files..." -ForegroundColor Yellow
Write-Host ""

if (-not (Test-Path "anticheat_files")) {
    New-Item -ItemType Directory -Path "anticheat_files" | Out-Null
}

$exePaths = @(
    "build\Release\anticheat-client.exe",
    "build\anticheat-client.exe"
)

$exeCopied = $false
foreach ($exePath in $exePaths) {
    if (Test-Path $exePath) {
        Copy-Item -Path $exePath -Destination "anticheat_files\" -Force
        Write-Host "[+] Copied anticheat-client.exe" -ForegroundColor Green
        $exeCopied = $true
        break
    }
}

if (-not $exeCopied) {
    Write-Host "[!] Warning: anticheat-client.exe not found in expected location" -ForegroundColor Yellow
}

if (Test-Path "release-config.json") {
    Copy-Item -Path "release-config.json" -Destination "anticheat_files\" -Force
    Write-Host "[+] Copied configuration file" -ForegroundColor Green
}

Write-Host ""

# Step 3: Build the launcher executable
Write-Host "[3/3] Building launcher executable..." -ForegroundColor Yellow
Write-Host ""

try {
    $pythonVersion = & python --version 2>&1
    Write-Host "[*] Found Python: $pythonVersion" -ForegroundColor Gray
    
    & python build_launcher.py
    if ($LASTEXITCODE -ne 0) {
        Write-Host "[!] Warning: Launcher build failed" -ForegroundColor Yellow
    }
    else {
        if (Test-Path "dist\AntiCheatLauncher.exe") {
            Copy-Item -Path "dist\AntiCheatLauncher.exe" -Destination "." -Force
            Write-Host ""
            Write-Host "[+] Launcher executable ready: AntiCheatLauncher.exe" -ForegroundColor Green
        }
    }
}
catch {
    Write-Host "[!] Warning: Python not found. Skipping launcher build." -ForegroundColor Yellow
    Write-Host "[!] Install Python 3.7+ to build the launcher executable." -ForegroundColor Yellow
}

Write-Host ""
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host " Build Complete" -ForegroundColor Cyan
Write-Host "================================================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "You can now run:" -ForegroundColor White
Write-Host "  - anticheat_files\anticheat-client.exe (direct execution)" -ForegroundColor Gray
Write-Host "  - AntiCheatLauncher.exe (launcher that manages downloads)" -ForegroundColor Gray
Write-Host ""
