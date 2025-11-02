@echo off
REM Quick run script - uses Python launcher directly
REM Use build_all.bat first to build the project

echo Starting Anti-Cheat Launcher...
echo.

if exist "AntiCheatLauncher.exe" (
    echo [*] Running standalone launcher executable...
    AntiCheatLauncher.exe
) else if exist "launcher.py" (
    echo [*] Running Python launcher script...
    python launcher.py
) else (
    echo [!] Error: No launcher found
    echo [!] Please run build_all.bat first
    pause
    exit /b 1
)
