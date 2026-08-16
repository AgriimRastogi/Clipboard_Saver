#include <windows.h>
#include <initguid.h>
#include <knownfolders.h>
#include <shlobj.h>
#include <objbase.h>
#include <gdiplus.h>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <memory>



// ============================================================================
// RAII GUARDS
// ============================================================================

// RAII wrapper for Windows Clipboard management
class ClipboardGuard {
private:
    bool m_opened;

public:
    explicit ClipboardGuard(HWND hWnd = nullptr) : m_opened(false) {
        m_opened = (OpenClipboard(hWnd) != FALSE);
    }

    ~ClipboardGuard() {
        if (m_opened) {
            CloseClipboard();
        }
    }

    // Prevent copying
    ClipboardGuard(const ClipboardGuard&) = delete;
    ClipboardGuard& operator=(const ClipboardGuard&) = delete;

    bool isOpen() const { return m_opened; }
};

// RAII wrapper for GDI+ Initialization & Cleanup
class GdiplusGuard {
private:
    ULONG_PTR m_token;
    bool m_initialized;

public:
    GdiplusGuard() : m_token(0), m_initialized(false) {
        Gdiplus::GdiplusStartupInput startupInput;
        if (Gdiplus::GdiplusStartup(&m_token, &startupInput, nullptr) == Gdiplus::Ok) {
            m_initialized = true;
        }
    }

    ~GdiplusGuard() {
        if (m_initialized) {
            Gdiplus::GdiplusShutdown(m_token);
        }
    }

    // Prevent copying
    GdiplusGuard(const GdiplusGuard&) = delete;
    GdiplusGuard& operator=(const GdiplusGuard&) = delete;

    bool isInitialized() const { return m_initialized; }
};

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

// Expands environment variables in path string (e.g. %USERPROFILE%)
std::wstring ExpandEnvironmentPath(const wchar_t* inputPath) {
    if (!inputPath) return L"";
    DWORD requiredSize = ExpandEnvironmentStringsW(inputPath, nullptr, 0);
    if (requiredSize == 0) {
        return std::wstring(inputPath);
    }

    std::vector<wchar_t> buffer(requiredSize);
    DWORD result = ExpandEnvironmentStringsW(inputPath, buffer.data(), requiredSize);
    if (result == 0 || result > requiredSize) {
        return std::wstring(inputPath);
    }

    // Create std::wstring from null-terminated buffer
    return std::wstring(buffer.data());
}

// Resolves the official Windows Pictures\Clipboard directory.
// Automatically accounts for OneDrive redirection, custom user folders, or standard local paths.
std::filesystem::path GetDefaultSaveDirectory() {
    PWSTR pKnownPath = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Pictures, 0, nullptr, &pKnownPath))) {
        std::filesystem::path picturesDir(pKnownPath);
        CoTaskMemFree(pKnownPath);
        return picturesDir / L"Clipboard";
    }

    // Fallback if Windows Known Folders lookup fails
    std::wstring fallbackPath = ExpandEnvironmentPath(L"%USERPROFILE%\\Pictures\\Clipboard");
    return std::filesystem::path(fallbackPath);
}

// Retrieves the CLSID for a GDI+ image encoder given a MIME type (e.g. L"image/png")
bool GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    UINT num = 0;          // Number of image encoders
    UINT size = 0;         // Size of the image encoder array in bytes

    if (Gdiplus::GetImageEncodersSize(&num, &size) != Gdiplus::Ok || size == 0) {
        return false;
    }

    std::vector<BYTE> memory(size);
    auto* pImageCodecInfo = reinterpret_cast<Gdiplus::ImageCodecInfo*>(memory.data());

    if (Gdiplus::GetImageEncoders(num, size, pImageCodecInfo) != Gdiplus::Ok) {
        return false;
    }

    for (UINT j = 0; j < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            return true;
        }
    }

    return false;
}

// Generates a timestamp-based filename in the format: Clipboard_YYYY-MM-DD_HH-MM-SS.png
std::wstring GenerateTimestampFilename() {
    auto now = std::chrono::system_clock::now();
    std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
    std::tm tmNow{};
#if defined(_WIN32)
    localtime_s(&tmNow, &timeNow);
#else
    tmNow = *std::localtime(&timeNow);
#endif

    wchar_t buffer[64];
    swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t),
              L"Clipboard_%04d-%02d-%02d_%02d-%02d-%02d.png",
              tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday,
              tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);

    return std::wstring(buffer);
}

// Generates a unique file path, ensuring no existing file is ever overwritten
std::filesystem::path GetUniqueFilePath(const std::filesystem::path& dirPath, const std::wstring& baseFilename) {
    std::filesystem::path targetPath = dirPath / baseFilename;
    if (!std::filesystem::exists(targetPath)) {
        return targetPath;
    }

    std::wstring stem = targetPath.stem().wstring();      // e.g. Clipboard_2026-07-26_22-18-30
    std::wstring extension = targetPath.extension().wstring(); // e.g. .png

    int counter = 1;
    while (true) {
        wchar_t suffix[32];
        swprintf_s(suffix, sizeof(suffix) / sizeof(wchar_t), L"_%d", counter++);
        std::filesystem::path candidatePath = dirPath / (stem + suffix + extension);
        if (!std::filesystem::exists(candidatePath)) {
            return candidatePath;
        }
    }
}

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================
int main() {
    // 1. Initialize GDI+ using RAII guard
    GdiplusGuard gdiplus;
    if (!gdiplus.isInitialized()) {
        return 0; // Exit silently on failure
    }

    // 2. Open Windows Clipboard using RAII guard
    ClipboardGuard clipboard;
    if (!clipboard.isOpen()) {
        return 0; // Exit silently if clipboard cannot be opened
    }

    // 3. Check if an image format (CF_BITMAP) is present in the clipboard
    if (!IsClipboardFormatAvailable(CF_BITMAP)) {
        return 0; // Exit silently if no image is present
    }

    // 4. Retrieve bitmap handle from clipboard
    auto hBitmap = reinterpret_cast<HBITMAP>(GetClipboardData(CF_BITMAP));
    if (!hBitmap) {
        return 0; // Exit silently if clipboard handle retrieval failed
    }

    // 5. Create GDI+ Bitmap object from HBITMAP
    std::unique_ptr<Gdiplus::Bitmap> pBitmap(Gdiplus::Bitmap::FromHBITMAP(hBitmap, nullptr));
    if (!pBitmap || pBitmap->GetLastStatus() != Gdiplus::Ok) {
        return 0; // Exit silently if GDI+ bitmap creation failed
    }

    // 6. Get PNG Encoder CLSID
    CLSID pngClsid;
    if (!GetEncoderClsid(L"image/png", &pngClsid)) {
        return 0; // Exit silently if PNG encoder is unavailable
    }

    // 7. Resolve output directory (custom CLI argument if provided, or official Pictures\Clipboard)
    std::filesystem::path outputDir;
    int numArgs = 0;
    LPWSTR* argList = CommandLineToArgvW(GetCommandLineW(), &numArgs);
    if (argList != nullptr) {
        if (numArgs > 1 && wcslen(argList[1]) > 0) {
            std::wstring expandedCustom = ExpandEnvironmentPath(argList[1]);
            if (!expandedCustom.empty()) {
                outputDir = std::filesystem::path(expandedCustom);
            }
        }
        LocalFree(argList);
    }

    if (outputDir.empty()) {
        outputDir = GetDefaultSaveDirectory();
    }

    if (outputDir.empty()) {
        return 0;
    }

    std::error_code ec;
    if (!std::filesystem::exists(outputDir, ec)) {
        if (!std::filesystem::create_directories(outputDir, ec)) {
            return 0; // Exit silently if directory creation failed
        }
    }

    // 8. Generate timestamp-based unique filename (never overwriting existing files)
    std::wstring baseFilename = GenerateTimestampFilename();
    std::filesystem::path finalFilePath = GetUniqueFilePath(outputDir, baseFilename);

    // 9. Save image as PNG
    pBitmap->Save(finalFilePath.c_str(), &pngClsid, nullptr);

    // 10. Exit immediately
    return 0;
}
