# Quick Start Guide

## For Users (Just Want to Run It)

### Step 1: Get the Launcher
Download `AntiCheatLauncher.exe`

### Step 2: Run It
Double-click `AntiCheatLauncher.exe`

That's it! The launcher handles everything else automatically.

---

## For Developers (First Time Setup)

### Step 1: Prerequisites
- Install [Visual Studio 2019 or 2022](https://visualstudio.microsoft.com/) with C++ workload
- Install [CMake](https://cmake.org/download/) (version 3.20+)
- Install [Python 3.7+](https://www.python.org/downloads/)

### Step 2: Build Everything
Open PowerShell in the project directory:
```powershell
.\build_all.ps1
```

Or use Command Prompt:
```batch
build_all.bat
```

### Step 3: Run
```batch
run.bat
```

Done! You now have:
- `anticheat_files/anticheat-client.exe` - The main anti-cheat client
- `AntiCheatLauncher.exe` - The launcher executable

---

## Common Tasks

### Just Build the C++ Client
```bash
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

### Just Build the Launcher
```bash
python build_launcher.py
```

### Test Without Building Launcher
```bash
python launcher.py
```

### Quick Rebuild After Changes
```bash
cd build
cmake --build . --config Release
cd ..
run.bat
```

---

## Distribution

### Option 1: Include Files (No Downloads)
Package and distribute:
- `AntiCheatLauncher.exe`
- `anticheat_files/` folder
- `release-config.json`

Users run `AntiCheatLauncher.exe` - no internet needed!

### Option 2: Download on First Run
1. Upload `anticheat-client.exe` to your server or GitHub releases
2. Edit `release-config.json`:
   ```json
   {
     "files": [
       {
         "name": "anticheat-client.exe",
         "url": "https://your-server.com/anticheat-client.exe",
         "required": true
       }
     ]
   }
   ```
3. Distribute only:
   - `AntiCheatLauncher.exe`
   - `release-config.json`

Users run `AntiCheatLauncher.exe` - it downloads everything!

---

## Troubleshooting

**"CMake not found"**
- Install CMake from cmake.org
- Add to PATH during installation

**"Python not found"**
- Install Python from python.org
- Check "Add Python to PATH" during installation

**"Build failed"**
- Make sure Visual Studio is installed with C++ workload
- Try running from "Developer Command Prompt for VS"

**"AntiCheatLauncher.exe not created"**
- Install PyInstaller: `pip install pyinstaller`
- Run: `python build_launcher.py`

**"Anti-cheat client not found"**
- Build the C++ project first: `build_all.ps1`
- Or copy `anticheat-client.exe` to `anticheat_files/`

---

## What Gets Created

```
project/
├── AntiCheatLauncher.exe      ← Distribute this!
├── release-config.json         ← Configure downloads here
│
├── build/
│   └── Release/
│       └── anticheat-client.exe
│
└── anticheat_files/           ← Runtime files
    └── anticheat-client.exe
```

---

## Need More Help?

- See `README.md` for detailed documentation
- See `USAGE.md` for quick command reference
- See `IMPLEMENTATION_SUMMARY.md` for technical details

---

## One-Liner Summary

**Build:** `.\build_all.ps1` → **Run:** `.\run.bat` → **Done!** 🎉
