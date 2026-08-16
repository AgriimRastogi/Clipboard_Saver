<#
.SYNOPSIS
    Builds ClipboardSaver using CMake, g++ (MinGW), or MSVC.
#>

Write-Host "===================================================" -ForegroundColor Cyan
Write-Host "  Building ClipboardSaver" -ForegroundColor Cyan
Write-Host "===================================================" -ForegroundColor Cyan

# 1. Try CMake
$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if ($cmake) {
    Write-Host "[*] Found CMake. Building with CMake..." -ForegroundColor Yellow
    cmake -B build
    if ($LASTEXITCODE -eq 0) {
        cmake --build build --config Release
        if ($LASTEXITCODE -eq 0) {
            Write-Host "[OK] Build succeeded! Executable located in build\ or build\Release\" -ForegroundColor Green
            exit 0
        }
    }
}

# 2. Try g++ (MinGW)
$gpp = Get-Command g++ -ErrorAction SilentlyContinue
if ($gpp) {
    Write-Host "[*] Found g++ (MinGW). Compiling standalone binary..." -ForegroundColor Yellow
    & g++ -std=c++17 -O3 -mwindows main.cpp -lgdiplus -lole32 -lshell32 -luser32 -luuid -o ClipboardSaver.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "[OK] Build succeeded! Created ClipboardSaver.exe" -ForegroundColor Green
        exit 0
    }
}

# 3. Try MSVC cl.exe
$cl = Get-Command cl -ErrorAction SilentlyContinue
if ($cl) {
    Write-Host "[*] Found MSVC cl.exe. Compiling standalone binary..." -ForegroundColor Yellow
    & cl /std:c++17 /O2 /EHsc /W4 /Fe:ClipboardSaver.exe main.cpp /link /SUBSYSTEM:WINDOWS gdiplus.lib ole32.lib shell32.lib user32.lib
    if ($LASTEXITCODE -eq 0) {
        Write-Host "[OK] Build succeeded! Created ClipboardSaver.exe" -ForegroundColor Green
        exit 0
    }
}

Write-Host "[ERROR] No supported compiler found (CMake, g++, or MSVC cl.exe)." -ForegroundColor Red
Write-Host "Please install Visual Studio, MinGW-w64, or CMake and ensure they are on PATH." -ForegroundColor Red
exit 1
