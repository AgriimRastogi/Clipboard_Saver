cmake_minimum_required(VERSION 3.16)
project(ClipboardSaver VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add executable as a Windows GUI application (subsystem: windows) to prevent console window popup
add_executable(ClipboardSaver WIN32 main.cpp)

# Link necessary Windows system libraries
if (MSVC)
    target_link_libraries(ClipboardSaver PRIVATE gdiplus ole32 shell32 user32)
    target_compile_options(ClipboardSaver PRIVATE /W4 /O2 /EHsc)
else()
    target_link_libraries(ClipboardSaver PRIVATE -lgdiplus -lole32 -lshell32 -luser32 -luuid)
    target_compile_options(ClipboardSaver PRIVATE -Wall -Wextra -O3)
endif()

# Installation target
install(TARGETS ClipboardSaver RUNTIME DESTINATION bin)
