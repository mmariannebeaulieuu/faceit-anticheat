#!/usr/bin/env python3
"""
Anti-Cheat Launcher
Downloads and runs the anti-cheat client and kernel driver
"""

import os
import sys
import subprocess
import urllib.request
import hashlib
import json
from pathlib import Path
import tempfile
import shutil

VERSION = "1.0.0"
CONFIG_URL = "https://raw.githubusercontent.com/yourusername/yourrepo/main/release-config.json"

class AntiCheatLauncher:
    def __init__(self):
        self.base_dir = Path(os.path.dirname(os.path.abspath(__file__)))
        self.download_dir = self.base_dir / "anticheat_files"
        self.download_dir.mkdir(exist_ok=True)
        
        self.config_file = self.base_dir / "release-config.json"
        self.config = None
        
    def print_banner(self):
        print("=" * 60)
        print("  CS:GO Anti-Cheat Launcher v" + VERSION)
        print("=" * 60)
        print()
        
    def load_config(self):
        """Load configuration from local file or download it"""
        print("[*] Loading configuration...")
        
        if self.config_file.exists():
            print(f"[*] Using local config: {self.config_file}")
            with open(self.config_file, 'r') as f:
                self.config = json.load(f)
        else:
            print("[*] Local config not found, using default embedded config")
            self.config = {
                "version": "1.0.0",
                "files": [
                    {
                        "name": "anticheat-client.exe",
                        "url": "",
                        "required": True,
                        "checksum": ""
                    },
                    {
                        "name": "AntiCheatKernelDriver.sys",
                        "url": "",
                        "required": False,
                        "checksum": ""
                    }
                ]
            }
        
        print(f"[+] Configuration loaded: version {self.config.get('version', 'unknown')}")
        return True
        
    def verify_checksum(self, filepath, expected_checksum):
        """Verify SHA256 checksum of a file"""
        if not expected_checksum:
            return True
            
        sha256_hash = hashlib.sha256()
        with open(filepath, "rb") as f:
            for byte_block in iter(lambda: f.read(4096), b""):
                sha256_hash.update(byte_block)
        
        actual = sha256_hash.hexdigest()
        return actual == expected_checksum
        
    def download_file(self, url, destination):
        """Download a file with progress"""
        if not url or url == "":
            print(f"[!] No URL provided for {destination.name}")
            return False
            
        print(f"[*] Downloading {destination.name}...")
        print(f"    From: {url}")
        
        try:
            with urllib.request.urlopen(url, timeout=30) as response:
                total_size = int(response.headers.get('content-length', 0))
                downloaded = 0
                
                with open(destination, 'wb') as out_file:
                    while True:
                        chunk = response.read(8192)
                        if not chunk:
                            break
                        out_file.write(chunk)
                        downloaded += len(chunk)
                        
                        if total_size > 0:
                            percent = (downloaded / total_size) * 100
                            print(f"\r    Progress: {percent:.1f}%", end='')
                
                print()
                print(f"[+] Downloaded: {destination.name}")
                return True
                
        except Exception as e:
            print(f"[!] Error downloading {destination.name}: {e}")
            return False
            
    def ensure_files(self):
        """Download or verify all required files"""
        print("\n[*] Checking required files...")
        
        for file_info in self.config.get('files', []):
            filename = file_info['name']
            url = file_info.get('url', '')
            required = file_info.get('required', True)
            checksum = file_info.get('checksum', '')
            
            filepath = self.download_dir / filename
            
            if filepath.exists():
                print(f"[*] Found: {filename}")
                
                if checksum and not self.verify_checksum(filepath, checksum):
                    print(f"[!] Checksum mismatch for {filename}, re-downloading...")
                    filepath.unlink()
                else:
                    print(f"[+] Verified: {filename}")
                    continue
            
            if url and url != "":
                if not self.download_file(url, filepath):
                    if required:
                        print(f"[!] Failed to download required file: {filename}")
                        return False
                    else:
                        print(f"[!] Optional file not available: {filename}")
                        continue
                        
                if checksum and not self.verify_checksum(filepath, checksum):
                    print(f"[!] Checksum verification failed for {filename}")
                    if required:
                        return False
            else:
                print(f"[!] No download URL for: {filename}")
                if required:
                    print(f"[!] Missing required file: {filename}")
                    return False
        
        return True
        
    def check_local_build(self):
        """Check if there's a local build available"""
        build_dirs = ['build', 'out', 'bin', 'cmake-build-release']
        
        for build_dir in build_dirs:
            build_path = self.base_dir / build_dir
            if build_path.exists():
                exe_path = build_path / 'anticheat-client.exe'
                if exe_path.exists():
                    print(f"[+] Found local build: {exe_path}")
                    
                    dest_path = self.download_dir / 'anticheat-client.exe'
                    if not dest_path.exists() or exe_path.stat().st_mtime > dest_path.stat().st_mtime:
                        print(f"[*] Copying local build to runtime directory...")
                        shutil.copy2(exe_path, dest_path)
                    
                    return True
                    
        return False
        
    def run_anticheat(self):
        """Launch the anti-cheat client"""
        exe_path = self.download_dir / "anticheat-client.exe"
        
        if not exe_path.exists():
            print("[!] Anti-cheat client executable not found!")
            return False
            
        print(f"\n[*] Launching anti-cheat client...")
        print(f"[*] Executable: {exe_path}")
        print("-" * 60)
        
        try:
            result = subprocess.run([str(exe_path)], cwd=str(self.download_dir))
            return result.returncode == 0
        except Exception as e:
            print(f"[!] Error launching anti-cheat: {e}")
            return False
            
    def run(self):
        """Main launcher logic"""
        self.print_banner()
        
        if not self.load_config():
            print("[!] Failed to load configuration")
            return 1
            
        print(f"[*] Working directory: {self.download_dir}")
        
        if not self.check_local_build():
            print("[*] No local build found, will use downloaded files")
            
        if not self.ensure_files():
            print("\n[!] Failed to prepare required files")
            print("[!] Please ensure you have built the project or configured download URLs")
            return 1
            
        print("\n[+] All required files are ready")
        
        if not self.run_anticheat():
            print("\n[!] Anti-cheat client exited with error")
            return 1
            
        print("\n[+] Anti-cheat client completed successfully")
        return 0

def main():
    if sys.platform != 'win32':
        print("Error: This application is designed for Windows only")
        return 1
        
    launcher = AntiCheatLauncher()
    return launcher.run()

if __name__ == "__main__":
    try:
        exit_code = main()
    except KeyboardInterrupt:
        print("\n[!] Interrupted by user")
        exit_code = 130
    except Exception as e:
        print(f"\n[!] Unexpected error: {e}")
        import traceback
        traceback.print_exc()
        exit_code = 1
        
    input("\nPress Enter to exit...")
    sys.exit(exit_code)
