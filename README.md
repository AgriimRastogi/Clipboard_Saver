# 📋 ClipboardSaver

> **Ultra-fast, silent, zero-UI Windows utility to instantly save clipboard images directly to disk as PNG files.**

[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)]()
[![Platform](https://img.shields.io/badge/platform-Windows%2010%20%7C%2011-0078D6.svg)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

---

## 💡 Why ClipboardSaver?

Whenever you copy an image or take a snip (`Win + Shift + S`), saving it usually requires opening an image editor (like MS Paint), pasting, clicking Save, choosing a folder, and typing a filename.

**ClipboardSaver automates this in one single keystroke:**
1. Copy or snip any image (`Win + Shift + S`, `PrintScreen`, or browser right-click -> *Copy Image*).
2. Trigger `ClipboardSaver.exe` (via your preferred hotkey).
3. **Done!** The image is silently saved to your Pictures folder as a timestamped PNG file in milliseconds.

---

## ✨ Features

- ⚡ **Zero UI & Silent**: No command prompt popups, windows, or dialogs. Executes in milliseconds and terminates immediately.
- 🖼️ **High Quality GDI+ Encoding**: Encodes clipboard bitmaps (`CF_BITMAP`) directly to PNG format using native Windows GDI+.
- 🕒 **Smart Timestamp Naming**: Automatically names files in `Clipboard_YYYY-MM-DD_HH-MM-SS.png` format.
- 🛡️ **Collision Avoidance**: Never overwrites existing images—automatically appends `_1`, `_2`, etc., if a timestamp collides.
- 📂 **Flexible Output Path**: Defaults to `%USERPROFILE%\Pictures\Clipboard`, but accepts custom folder paths via command-line arguments and automatically expands environment variables (e.g. `%ONEDRIVE%`, `%USERPROFILE%`).
- 🔒 **RAII Resource Management**: Built with modern C++17 RAII guards, ensuring clipboard handles and GDI+ tokens are cleanly released under all circumstances.
- 🪶 **Ultra-Lightweight & Standalone**: Zero external dependencies, runtime frameworks, or DLL bloat (< 200 KB).

---

## 🚀 Quick Start

### 1. Download or Build
Download the latest `ClipboardSaver.exe` from [Releases](../../releases) or build it locally using `build.bat`.

### 2. Run
Simply run `ClipboardSaver.exe`. If an image is in your clipboard, it will immediately be saved to:
```
C:\Users\<YourUsername>\Pictures\Clipboard\
```

---

## ⌨️ Setting Up a Global Hotkey

To get the most out of ClipboardSaver, bind it to a hotkey so you can save images instantly without opening any software.

### Method 1: Native Windows Shortcut (Built-in & Zero Downloads)
1. Place `ClipboardSaver.exe` in a permanent folder (e.g., `C:\Tools\ClipboardSaver.exe` or `%USERPROFILE%\Tools`).
2. Right-click `ClipboardSaver.exe` → **Create shortcut**.
3. Move or keep the shortcut in a convenient place (like your Desktop or Start Menu).
4. Right-click the shortcut → **Properties**.
5. In the **Shortcut key** field, press your desired combination (e.g., `Ctrl + Alt + V` or `Ctrl + Alt + S`).
6. Click **Apply** and **OK**.

### Method 2: Microsoft PowerToys / Stream Deck (Optional)
- Use **PowerToys Keyboard Manager** (Remap a shortcut ➔ Launch program ➔ `ClipboardSaver.exe`).
- Or map a **Stream Deck / Macro Key** button to launch `ClipboardSaver.exe` with a single tap.

---

## ⚙️ Custom Folder Configuration

### Via Command-Line Argument
You can pass a custom target directory directly as an argument:
```cmd
ClipboardSaver.exe "D:\Screenshots"
```
Environment variables are automatically supported:
```cmd
ClipboardSaver.exe "%ONEDRIVE%\Pictures\Clipboard"
```

### In Source Code
You can customize the default fallback path by modifying `DEFAULT_SAVE_DIRECTORY` at the top of [`main.cpp`](main.cpp):
```cpp
const wchar_t* DEFAULT_SAVE_DIRECTORY = L"%USERPROFILE%\\Pictures\\Clipboard";
```

---

## 🛠️ Building from Source

### Prerequisites
- Windows 10 / 11
- A C++17 compatible compiler (MSVC, MinGW-w64 GCC, or Clang)
- *(Optional)* CMake 3.16+

### Option A: One-Click Build Script
Run the automated build script:
```cmd
build.bat
```

### Option B: CMake
```bash
cmake -B build
cmake --build build --config Release
```

### Option C: GCC / MinGW
```bash
g++ -std=c++17 -O3 -mwindows main.cpp -lgdiplus -lole32 -lshell32 -luser32 -o ClipboardSaver.exe
```

### Option D: MSVC (`cl.exe`)
From a *x64 Native Tools Command Prompt for VS*:
```cmd
cl /std:c++17 /O2 /EHsc /W4 /Fe:ClipboardSaver.exe main.cpp /link /SUBSYSTEM:WINDOWS gdiplus.lib ole32.lib shell32.lib user32.lib
```

---

## 📁 Repository Structure

```
├── .github/
│   └── workflows/
│       └── build.yml       # GitHub Actions CI build & release workflow
├── .gitignore              # Git ignore rules for C++ / CMake / Visual Studio
├── build.bat               # Automated build script (Command Prompt)
├── build.ps1               # Automated build script (PowerShell)
├── CMakeLists.txt          # CMake configuration
├── CONTRIBUTING.md         # Contribution guidelines
├── LICENSE                 # MIT License
├── main.cpp                # Source code
└── README.md               # Project documentation
```

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request or open an Issue. See [`CONTRIBUTING.md`](CONTRIBUTING.md) for details.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).
