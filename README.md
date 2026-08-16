# ClipboardSaver

A tiny, silent Windows utility that takes whatever image is in your clipboard and saves it directly to your `Pictures\Clipboard` folder as a timestamped PNG. No UI, no windows, no popups.

---

## ⬇️ Download & Setup

1. Grab **`ClipboardSaver.exe`** from the [Releases](../../releases) tab.
2. Put it anywhere you want (e.g. `C:\Tools\` or inside your user folder).

### Set up a Hotkey (e.g. `Ctrl + Alt + V`)
1. Right-click `ClipboardSaver.exe` ➔ **Create shortcut**.
2. Right-click the shortcut ➔ **Properties**.
3. Click in the **Shortcut key** field and press `Ctrl + Alt + V` (or whatever keys you prefer).
4. Click **OK**.

Now whenever you copy an image or take a screenshot (`Win + Shift + S`), just hit your hotkey and it's instantly saved to your official **`Pictures\Clipboard`** folder (automatically detecting OneDrive or custom drive paths).

---

## ⚙️ Custom Folder (Optional)

If you want images saved somewhere else instead of your default Pictures folder, just pass the path as an argument:

```cmd
ClipboardSaver.exe "D:\Screenshots"
```

---

## 🛠️ Build from Source

If you prefer building it yourself (requires C++17):

**Using the build script:**
```cmd
build.bat
```

**Or using g++ directly:**
```bash
g++ -std=c++17 -O3 -mwindows main.cpp -lgdiplus -lole32 -lshell32 -luser32 -luuid -o ClipboardSaver.exe
```

---

## License

[MIT](LICENSE)
