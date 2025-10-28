# Quick Usage Guide

## For End Users

### Running the Anti-Cheat

1. **Download** `AntiCheatLauncher.exe` 
2. **Run** `AntiCheatLauncher.exe`
3. The launcher will automatically:
   - Download required files
   - Verify integrity
   - Start the anti-cheat

That's it! The launcher handles everything.

## For Developers

### First Time Setup

```bash
# 1. Build everything (C++ client + launcher executable)
.\build_all.ps1
# or
build_all.bat

# 2. Run the anti-cheat
.\run.bat
# or
.\AntiCheatLauncher.exe
```

### Quick Commands

```bash
# Build only the C++ client
cd build
cmake --build . --config Release

# Build only the launcher executable
python build_launcher.py

# Run with Python (no exe build needed)
python launcher.py

# Run the compiled launcher
.\AntiCheatLauncher.exe

# Direct run (bypass launcher)
.\anticheat_files\anticheat-client.exe
```

## Deployment Workflow

### Option 1: Local Distribution (No Downloads)

1. Build the project:
   ```bash
   .\build_all.ps1
   ```

2. Distribute these files:
   ```
   AntiCheatLauncher.exe
   anticheat_files/anticheat-client.exe
   release-config.json (with empty URLs)
   ```

3. Users run `AntiCheatLauncher.exe`

### Option 2: Remote Distribution (With Downloads)

1. Build the project:
   ```bash
   .\build_all.ps1
   ```

2. Upload files to your server or GitHub releases:
   - `anticheat-client.exe`
   - `AntiCheatKernelDriver.sys` (optional)

3. Update `release-config.json` with URLs:
   ```json
   {
     "files": [
       {
         "name": "anticheat-client.exe",
         "url": "https://yourserver.com/anticheat-client.exe",
         "required": true
       }
     ]
   }
   ```

4. Distribute only:
   ```
   AntiCheatLauncher.exe
   release-config.json
   ```

5. Users run `AntiCheatLauncher.exe` - it downloads everything else

## File Structure

```
project/
├── AntiCheatLauncher.exe      # Standalone launcher (distribute this)
├── release-config.json         # Download configuration
├── launcher.py                 # Launcher source code
├── build_all.bat/ps1          # Build scripts
├── run.bat                     # Quick run script
│
├── src/                        # C++ source code
├── include/                    # C++ headers
├── kernel/                     # Kernel driver source
│
├── build/                      # CMake build directory
│   └── Release/
│       └── anticheat-client.exe
│
└── anticheat_files/           # Runtime directory (auto-created)
    ├── anticheat-client.exe   # Main executable
    └── release-config.json    # Local config copy
```

## Configuration

### Setting Download URLs

Edit `release-config.json`:

```json
{
  "version": "1.0.0",
  "files": [
    {
      "name": "anticheat-client.exe",
      "url": "https://github.com/user/repo/releases/download/v1.0.0/anticheat-client.exe",
      "required": true,
      "checksum": ""
    }
  ]
}
```

### Calculating Checksums

```bash
# Windows PowerShell
Get-FileHash -Algorithm SHA256 anticheat-client.exe

# Then add the hash to release-config.json
```

## Common Scenarios

### Scenario 1: Testing Locally

```bash
.\build_all.ps1
python launcher.py
```

### Scenario 2: Building for Distribution

```bash
.\build_all.ps1
# Then distribute: AntiCheatLauncher.exe + release-config.json
```

### Scenario 3: Quick Rebuild After Code Changes

```bash
cd build
cmake --build . --config Release
cd ..
python launcher.py
```

### Scenario 4: Update Launcher Only

```bash
# Edit launcher.py
python build_launcher.py
# New AntiCheatLauncher.exe is in dist/
```

## Tips

- **Development**: Use `python launcher.py` for quick testing
- **Production**: Build with `python build_launcher.py` for distribution
- **Local testing**: Keep URLs empty in `release-config.json`
- **Remote testing**: Host files and update URLs in config
- **Checksums**: Optional but recommended for production

## Troubleshooting

**"anticheat-client.exe not found"**
- Run `build_all.bat` or `build_all.ps1` first
- Or copy the exe manually to `anticheat_files/`

**Download fails**
- Check URLs in `release-config.json`
- Verify internet connection
- Use local files by keeping URLs empty

**Python not found**
- Install Python 3.7+ from python.org
- Or use pre-built `AntiCheatLauncher.exe`

**Build fails**
- Install Visual Studio with C++ workload
- Update CMake to 3.20+
- Check Windows SDK is installed
