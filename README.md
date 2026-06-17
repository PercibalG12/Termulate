# Termulate

- Author: Percival Roberts
- Liscense: MIT

A simple cross-platform terminal emulator written in C using GTK.

## Requirements

### Windows

Install:

* MSYS2
* GTK3
* GCC

### Install MSYS2

Download and install:

https://www.msys2.org/

Open:

```text
MSYS2 UCRT64
```

Update packages:

```bash
pacman -Syu
```

Install GTK:

```bash
pacman -S mingw-w64-ucrt-x86_64-gtk3
```

Verify installation:

```bash
pkg-config --cflags gtk+-3.0
```

You should see multiple GTK include paths returned.

---

## VS Code Setup

Install:

* C/C++ Extension (Microsoft)

Open the project folder.

Create or update:

```text
.vscode/c_cpp_properties.json
```

Example configuration:

```json
{
    "configurations": [
        {
            "name": "MSYS2 UCRT64",
            "includePath": [
                "${workspaceFolder}/**",
                "C:/msys64/ucrt64/include",
                "C:/msys64/ucrt64/include/gtk-3.0",
                "C:/msys64/ucrt64/include/glib-2.0",
                "C:/msys64/ucrt64/lib/glib-2.0/include",
                "C:/msys64/ucrt64/include/pango-1.0",
                "C:/msys64/ucrt64/include/harfbuzz",
                "C:/msys64/ucrt64/include/cairo",
                "C:/msys64/ucrt64/include/gdk-pixbuf-2.0",
                "C:/msys64/ucrt64/include/atk-1.0"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "cStandard": "c17",
            "cppStandard": "gnu++17",
            "intelliSenseMode": "windows-gcc-x64",
            "compilerPath": "C:/msys64/ucrt64/bin/gcc.exe"
        }
    ],
    "version": 4
}
```

After saving:

1. Ctrl + Shift + P
2. Run:

```text
C/C++: Reset IntelliSense Database
```

3. Run:

```text
Developer: Reload Window
```

This should remove include errors such as:

```c
#include <gtk/gtk.h>
```

---

## Project Structure

```text
Termulate/
│
├── main.c
├── commands.c
├── commands.h
├── README.txt
│
└── .vscode
    └── c_cpp_properties.json
```

---

## Building

### Method 1 (Recommended) - MSYS2 UCRT64 Terminal

Navigate to project:

```bash
cd /c/Users/yourusername/Documents/clients/Termulate
```

Compile:

```bash
gcc main.c commands.c -o termulate.exe $(pkg-config --cflags --libs gtk+-3.0)
```

Run:

```bash
./termulate.exe
```

---

### Method 2 - Windows CMD

Navigate to project:

```cmd
cd C:\Users\perci\Documents\clients\Termulate
```

Compile:

```cmd
for /f "usebackq tokens=*" %i in (`pkg-config --cflags --libs gtk+-3.0`) do gcc main.c commands.c -o termulate.exe %i
```

Run:

```cmd
termulate.exe
```

---

### Method 3 - PowerShell

Navigate to project:

```powershell
cd C:\Users\perci\Documents\clients\Termulate
```

Compile:

```powershell
gcc main.c commands.c -o termulate.exe $(pkg-config --cflags --libs gtk+-3.0)
```


Run:

```powershell
.\termulate.exe
```

---

## Build Script (Recommended)

Instead of typing the full compile command every time, create a build script.

### Windows CMD - build.bat

Create a file named:

```text
build.bat
```

Add:

```bat
@echo off

for /f "usebackq tokens=*" %%i in (`pkg-config --cflags --libs gtk+-3.0`) do (
    gcc main.c commands.c -o termulate.exe %%i
)

echo.
echo Build complete.
```

Run:

```cmd
build.bat
```

This will automatically compile the project and generate:

```text
termulate.exe
```

---

### PowerShell - build.ps1

Create a file named:

```text
build.ps1
```

Add:

```powershell
gcc main.c commands.c -o termulate.exe $(pkg-config --cflags --libs gtk+-3.0)

Write-Host ""
Write-Host "Build complete."
```

Run:

```powershell
.\build.ps1
```

---

### MSYS2 - build.sh

Create a file named:

```bash
build.sh
```

Add:

```bash
#!/bin/bash

gcc main.c commands.c -o termulate.exe $(pkg-config --cflags --libs gtk+-3.0)

echo ""
echo "Build complete."
```

Make executable:

```bash
chmod +x build.sh
```

Run:

```bash
./build.sh
```

---

### Recommended Workflow

During development:

1. Open VS Code
2. Edit source files
3. Open MSYS2 UCRT64 terminal
4. Run:

```bash
./build.sh
```

or on Windows:

```cmd
build.bat
```

5. Launch:

```text
./termulate.exe
```

This avoids having to remember the full GTK compile command every time.
---

## Troubleshooting

### Error

```text
gtk/gtk.h: No such file or directory
```

Solution:

Verify GTK is installed:

```bash
pkg-config --cflags gtk+-3.0
```

If paths are returned, GTK is installed correctly.

---

### VS Code shows red squiggly line under gtk.h

This is an IntelliSense issue.

Check:

```text
compilerPath
```

is set to:

```text
C:/msys64/ucrt64/bin/gcc.exe
```

Then reset IntelliSense.

---

## Current Commands

```text
help
ls
dir
clear
exit
```

## Future Commands

```text
cd
mkdir
rmdir
copy
move
cat
pwd
history
```
