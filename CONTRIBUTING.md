# Contributing to ClipboardSaver

Thank you for your interest in improving ClipboardSaver! Contributions of all kinds are welcome: bug fixes, feature suggestions, documentation enhancements, and workflow integrations.

## Code Guidelines

1. **C++ Standard**: We target modern **C++17**.
2. **Zero Dependencies**: Rely only on native Windows Win32 / GDI+ APIs to keep the binary ultra-lightweight (< 200 KB) and standalone with zero external DLLs.
3. **RAII Safety**: Always wrap Windows OS handles (`OpenClipboard`, `GDI+ Startup`, etc.) in RAII guard objects to guarantee resource cleanup.
4. **Silent Execution**: Unless explicit CLI flags are added, the default behavior must remain completely silent (no console windows, popups, or message boxes).

## Development Workflow

1. Fork the repository.
2. Clone your fork locally:
   ```bash
   git clone https://github.com/your-username/ClipboardSaver.git
   cd ClipboardSaver
   ```
3. Create a feature branch:
   ```bash
   git checkout -b feature/my-new-feature
   ```
4. Build and test your changes:
   ```bash
   cmake -B build
   cmake --build build
   ```
   Or simply run `build.bat`.
5. Commit your changes and open a Pull Request.

## Reporting Issues

If you encounter a bug or have a suggestion, please open an Issue with:
- Windows version (e.g., Windows 10, Windows 11).
- Steps to reproduce the issue.
- Expected vs. actual behavior.
