@echo off
setlocal

echo ===================================================
echo   Building ClipboardSaver
echo ===================================================

:: Check for CMake first
where cmake >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo [*] Found CMake. Building with CMake...
    cmake -B build
    if %ERRORLEVEL% equ 0 (
        cmake --build build --config Release
        if %ERRORLEVEL% equ 0 (
            echo [OK] Build succeeded. Executable located in build\ or build\Release\
            goto done
        )
    )
)

:: Fallback to g++ (MinGW)
where g++ >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo [*] Found g++ (MinGW). Compiling standalone binary...
    g++ -std=c++17 -O3 -mwindows main.cpp -lgdiplus -lole32 -lshell32 -luser32 -o ClipboardSaver.exe
    if %ERRORLEVEL% equ 0 (
        echo [OK] Build succeeded. Created ClipboardSaver.exe
        goto done
    )
)

:: Fallback to MSVC cl.exe
where cl >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo [*] Found MSVC cl.exe. Compiling standalone binary...
    cl /std:c++17 /O2 /EHsc /W4 /Fe:ClipboardSaver.exe main.cpp /link /SUBSYSTEM:WINDOWS gdiplus.lib ole32.lib shell32.lib user32.lib
    if %ERRORLEVEL% equ 0 (
        echo [OK] Build succeeded. Created ClipboardSaver.exe
        goto done
    )
)

echo [ERROR] No supported compiler found (CMake, g++, or MSVC cl.exe).
echo Please install Visual Studio, MinGW-w64, or CMake and ensure they are on PATH.
exit /b 1

:done
echo ===================================================
echo   Done
echo ===================================================
endlocal
exit /b 0
