@echo off
REM Quick Start - Build and Run in one command
REM This is useful for development and testing

echo ================================================================
echo  Anti-Cheat Quick Start
echo ================================================================
echo.

REM Check if already built
if exist "anticheat_files\anticheat-client.exe" (
    echo [*] Found existing build
    goto :run_launcher
)

echo [*] No existing build found, building now...
echo.

REM Build if not exists
call build_all.bat
if errorlevel 1 (
    echo [!] Build failed
    pause
    exit /b 1
)

:run_launcher
echo.
echo [*] Starting launcher...
echo.

if exist "AntiCheatLauncher.exe" (
    AntiCheatLauncher.exe
) else if exist "launcher.py" (
    python launcher.py
) else (
    echo [!] No launcher found
    pause
    exit /b 1
)
