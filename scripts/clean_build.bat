@echo off
REM Remove and recreate build directory (Windows)
if exist build (
    rmdir /s /q build
)
mkdir build
echo Recreated build directory: %cd%\build
