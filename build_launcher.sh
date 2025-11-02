#!/bin/bash
# Build script to create the launcher executable using PyInstaller
# Note: This creates a Windows .exe even on Linux (requires Wine for PyInstaller)

set -e

echo "================================================================"
echo "  Building Anti-Cheat Launcher Executable"
echo "================================================================"
echo ""

# Check if Python is available
if ! command -v python3 &> /dev/null; then
    echo "[!] Error: Python 3 not found"
    exit 1
fi

# Check if PyInstaller is installed
if ! python3 -c "import PyInstaller" 2>/dev/null; then
    echo "[!] PyInstaller not found. Installing..."
    python3 -m pip install pyinstaller
    echo "[+] PyInstaller installed"
else
    echo "[+] PyInstaller found"
fi

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LAUNCHER_SCRIPT="$PROJECT_DIR/launcher.py"
CONFIG_FILE="$PROJECT_DIR/release-config.json"

if [ ! -f "$LAUNCHER_SCRIPT" ]; then
    echo "[!] Error: launcher.py not found at $LAUNCHER_SCRIPT"
    exit 1
fi

echo "[*] Building executable from: $LAUNCHER_SCRIPT"

CMD=(
    pyinstaller
    --onefile
    --console
    --name AntiCheatLauncher
    --clean
)

if [ -f "$CONFIG_FILE" ]; then
    CMD+=(--add-data "$CONFIG_FILE:.")
    echo "[*] Including config file: $CONFIG_FILE"
fi

CMD+=("$LAUNCHER_SCRIPT")

echo "[*] Running: ${CMD[*]}"
echo ""

cd "$PROJECT_DIR"
"${CMD[@]}"

if [ $? -eq 0 ]; then
    DIST_DIR="$PROJECT_DIR/dist"
    if [ -f "$DIST_DIR/AntiCheatLauncher.exe" ]; then
        EXE_PATH="$DIST_DIR/AntiCheatLauncher.exe"
        EXE_SIZE=$(du -h "$EXE_PATH" | cut -f1)
        echo ""
        echo "================================================================"
        echo "[+] Build successful!"
        echo "[+] Executable created: $EXE_PATH"
        echo "[+] Size: $EXE_SIZE"
        echo "================================================================"
    elif [ -f "$DIST_DIR/AntiCheatLauncher" ]; then
        EXE_PATH="$DIST_DIR/AntiCheatLauncher"
        EXE_SIZE=$(du -h "$EXE_PATH" | cut -f1)
        echo ""
        echo "================================================================"
        echo "[+] Build successful!"
        echo "[+] Executable created: $EXE_PATH"
        echo "[+] Size: $EXE_SIZE"
        echo "[!] Note: Built for current platform, not Windows"
        echo "================================================================"
    else
        echo "[!] Build completed but executable not found"
        exit 1
    fi
else
    echo "[!] Build failed"
    exit 1
fi
