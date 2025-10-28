#!/usr/bin/env python3
"""
Build script to create the launcher executable using PyInstaller
"""

import subprocess
import sys
import os
from pathlib import Path

def main():
    print("=" * 60)
    print("  Building Anti-Cheat Launcher Executable")
    print("=" * 60)
    print()
    
    try:
        import PyInstaller
        print(f"[+] PyInstaller found: {PyInstaller.__version__}")
    except ImportError:
        print("[!] PyInstaller not found. Installing...")
        subprocess.check_call([sys.executable, "-m", "pip", "install", "pyinstaller"])
        print("[+] PyInstaller installed")
    
    project_dir = Path(__file__).parent
    launcher_script = project_dir / "launcher.py"
    config_file = project_dir / "release-config.json"
    
    if not launcher_script.exists():
        print(f"[!] Error: launcher.py not found at {launcher_script}")
        return 1
    
    print(f"[*] Building executable from: {launcher_script}")
    
    cmd = [
        "pyinstaller",
        "--onefile",
        "--console",
        "--name", "AntiCheatLauncher",
        "--clean",
    ]
    
    if config_file.exists():
        cmd.extend(["--add-data", f"{config_file}{os.pathsep}."])
        print(f"[*] Including config file: {config_file}")
    
    cmd.append(str(launcher_script))
    
    print(f"[*] Running: {' '.join(cmd)}")
    print()
    
    result = subprocess.run(cmd, cwd=str(project_dir))
    
    if result.returncode == 0:
        dist_dir = project_dir / "dist"
        exe_path = dist_dir / "AntiCheatLauncher.exe"
        
        if exe_path.exists():
            print()
            print("=" * 60)
            print("[+] Build successful!")
            print(f"[+] Executable created: {exe_path}")
            print(f"[+] Size: {exe_path.stat().st_size / 1024 / 1024:.2f} MB")
            print("=" * 60)
            return 0
        else:
            print("[!] Build completed but executable not found")
            return 1
    else:
        print("[!] Build failed")
        return result.returncode

if __name__ == "__main__":
    sys.exit(main())
