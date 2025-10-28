# CS:GO Anti-Cheat Prototype

A Windows-based anti-cheat system with user-mode and kernel-mode components.

## Project Structure

- `src/` - User-mode anti-cheat client source code
- `include/` - Header files for the client
- `kernel/` - Kernel-mode driver source code
- `shared/` - Shared protocol definitions
- `launcher.py` - Python-based launcher that downloads and runs the anti-cheat
- `build_launcher.py` - Script to build the launcher as a standalone .exe
- `release-config.json` - Configuration for downloadable files

## Building the Project

### Prerequisites

**For C++ Client:**
- Windows 10/11
- Visual Studio 2019 or 2022 (with C++ desktop development workload)
- CMake 3.20 or higher

**For Launcher Executable:**
- Python 3.7 or higher
- PyInstaller (installed automatically by build script)

### Quick Build (Recommended)

Use the provided build scripts to build everything:

**Using PowerShell:**
```powershell
.\build_all.ps1
```

**Using Command Prompt:**
```batch
build_all.bat
```

### Manual Build

#### Build C++ Client

```bash
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

The executable will be in `build/Release/anticheat-client.exe`

#### Build Launcher Executable

```bash
python build_launcher.py
```

The launcher executable will be in `dist/AntiCheatLauncher.exe`

## Running the Anti-Cheat

### Method 1: Using the Launcher (Recommended)

The launcher executable provides automatic file management:

1. Run `AntiCheatLauncher.exe`
2. The launcher will:
   - Check for local builds
   - Download missing files (if URLs are configured)
   - Verify file integrity
   - Launch the anti-cheat client

### Method 2: Direct Execution

Run the client directly:

```bash
.\anticheat_files\anticheat-client.exe
```

## Launcher Configuration

The `release-config.json` file controls what files the launcher downloads:

```json
{
  "version": "1.0.0",
  "files": [
    {
      "name": "anticheat-client.exe",
      "url": "https://your-server.com/files/anticheat-client.exe",
      "required": true,
      "checksum": "sha256-hash-here"
    }
  ]
}
```

### Configuration Fields

- `name` - Filename to save as
- `url` - Download URL (leave empty to skip download)
- `required` - Whether the file is required to run
- `checksum` - SHA256 hash for verification (optional)

## Deployment

### Creating a Distributable Package

1. Build the project using `build_all.ps1` or `build_all.bat`
2. Host your files on a web server or GitHub releases
3. Update `release-config.json` with the actual URLs
4. Distribute `AntiCheatLauncher.exe` and `release-config.json`

### GitHub Releases Setup

1. Create a GitHub release with your built files
2. Update URLs in `release-config.json`:
   ```json
   "url": "https://github.com/username/repo/releases/download/v1.0.0/anticheat-client.exe"
   ```
3. Users run the launcher, which downloads from your release

### Self-Contained Distribution

To distribute without requiring downloads:

1. Build the project
2. Package these files together:
   - `AntiCheatLauncher.exe`
   - `anticheat_files/anticheat-client.exe`
   - `release-config.json` (with empty URLs)

The launcher will detect and use local files.

## Features

### User-Mode Client (`anticheat-client.exe`)

- Process enumeration and monitoring
- Digital signature verification
- Memory signature scanning for known cheats
- Certificate trust validation
- Kernel driver interface (optional)

### Kernel Driver (`AntiCheatKernelDriver.sys`)

- Deep system monitoring (optional component)
- Requires administrator privileges
- Must be signed for production use

### Launcher (`AntiCheatLauncher.exe`)

- Automatic file downloads
- SHA256 integrity verification
- Local build detection
- Graceful fallback to local files
- Progress reporting

## Development

### Adding New Files to Download

Edit `release-config.json`:

```json
{
  "files": [
    {
      "name": "new-file.dll",
      "url": "https://example.com/new-file.dll",
      "required": false,
      "checksum": ""
    }
  ]
}
```

### Customizing the Launcher

Edit `launcher.py` and rebuild with `build_launcher.py`.

## Security Notes

- The kernel driver requires code signing for Windows 10/11
- Always verify checksums when downloading files
- Use HTTPS URLs for downloads
- Keep `release-config.json` in a trusted location

## Troubleshooting

### Launcher Issues

**"Anti-cheat client executable not found"**
- Ensure you've built the project or configured download URLs
- Check that `anticheat_files/anticheat-client.exe` exists

**Download failures**
- Check your internet connection
- Verify URLs in `release-config.json` are correct
- Ensure the files are publicly accessible

### Build Issues

**CMake configuration fails**
- Install Visual Studio with C++ workload
- Update CMake to version 3.20 or higher

**PyInstaller not found**
- Install Python 3.7+
- Run: `pip install pyinstaller`

## License

Educational prototype - see project documentation for details.

## Requirements

- Windows 10/11 (x64)
- Administrator privileges (for kernel driver only)
- Internet connection (for downloading files)
