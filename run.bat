@echo off
if not exist build\CSafeMEMZ.exe (
  echo CSafeMEMZ.exe not built yet.
  echo Run build.bat first.
  pause
  exit /b 1
)
build\CSafeMEMZ.exe
