@echo off
REM Build script for Anti-Cheat project (Windows)
REM This script builds both the C++ client and the Python launcher executable

echo ================================================================
echo  Building Anti-Cheat Project
echo ================================================================
echo.

REM Check if we're in the right directory
if not exist "CMakeLists.txt" (
    echo Error: CMakeLists.txt not found. Please run this from the project root.
    exit /b 1
)

REM Step 1: Build the C++ Anti-Cheat Client
echo [1/3] Building C++ Anti-Cheat Client...
echo.

if not exist "build" mkdir build
cd build

cmake -G "Visual Studio 17 2022" -A x64 ..
if errorlevel 1 (
    echo Error: CMake configuration failed
    cd ..
    exit /b 1
)

cmake --build . --config Release
if errorlevel 1 (
    echo Error: Build failed
    cd ..
    exit /b 1
)

cd ..
echo.
echo [+] C++ client built successfully
echo.

REM Step 2: Copy built files to distribution directory
echo [2/3] Preparing distribution files...
echo.

if not exist "anticheat_files" mkdir anticheat_files

if exist "build\Release\anticheat-client.exe" (
    copy /Y "build\Release\anticheat-client.exe" "anticheat_files\"
    echo [+] Copied anticheat-client.exe
) else if exist "build\anticheat-client.exe" (
    copy /Y "build\anticheat-client.exe" "anticheat_files\"
    echo [+] Copied anticheat-client.exe
) else (
    echo [!] Warning: anticheat-client.exe not found in expected location
)

if exist "release-config.json" (
    copy /Y "release-config.json" "anticheat_files\"
    echo [+] Copied configuration file
)

echo.

REM Step 3: Build the launcher executable
echo [3/3] Building launcher executable...
echo.

python --version >nul 2>&1
if errorlevel 1 (
    echo [!] Warning: Python not found. Skipping launcher build.
    echo [!] Install Python 3.7+ to build the launcher executable.
    goto :skip_launcher
)

python build_launcher.py
if errorlevel 1 (
    echo [!] Warning: Launcher build failed
    goto :skip_launcher
)

if exist "dist\AntiCheatLauncher.exe" (
    copy /Y "dist\AntiCheatLauncher.exe" "."
    echo.
    echo [+] Launcher executable ready: AntiCheatLauncher.exe
)

:skip_launcher

echo.
echo ================================================================
echo  Build Complete
echo ================================================================
echo.
echo You can now run:
echo   - anticheat_files\anticheat-client.exe (direct execution)
echo   - AntiCheatLauncher.exe (launcher that manages downloads)
echo.
pause
