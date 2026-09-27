@echo off
setlocal
cd /d "%~dp0"

echo ========================================
echo       CSafeMEMZ %CSAFE_VERSION%
echo       SAFE VISUAL SIMULATION
 echo ========================================

echo.
where gcc >nul 2>nul
if %errorlevel%==0 goto :gcc

where clang-cl >nul 2>nul
if %errorlevel%==0 goto :clang

echo [ERROR] MinGW gcc or clang-cl was not found.
echo Install Visual Studio Build Tools or MinGW-w64.
pause
exit /b 1

:gcc
if not exist build mkdir build
gcc -O2 -Wall -Wextra -std=c11 -DWIN32_LEAN_AND_MEAN -mwindows ^
    -Iinclude -Isrc/core -Isrc/effects -Isrc/ui ^
    src/main.c src/core/engine.c src/core/event.c src/core/timeline.c ^
    src/effects/effects.c src/ui/fake_windows.c src/ui/outro.c ^
    -o build\CSafeMEMZ.exe -lgdi32 -luser32
if errorlevel 1 goto :fail
echo.
echo BUILD OK: build\CSafeMEMZ.exe
goto :done

:clang
if not exist build mkdir build
clang-cl /O2 /DWIN32_LEAN_AND_MEAN /Iinclude /Isrc/core /Isrc/effects /Isrc/ui ^
    src\main.c src\core\engine.c src\core\event.c src\core\timeline.c ^
    src\effects\effects.c src\ui\fake_windows.c src\ui\outro.c ^
    /Fe:build\CSafeMEMZ.exe user32.lib gdi32.lib /link /SUBSYSTEM:WINDOWS
if errorlevel 1 goto :fail
echo.
echo BUILD OK: build\CSafeMEMZ.exe
goto :done

:fail
echo.
echo BUILD FAILED.
pause
exit /b 1

:done
echo.
echo Run: build\CSafeMEMZ.exe
echo Emergency exit: ESC
pause
