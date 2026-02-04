@echo off
REM Build helper (Windows)
setlocal

pushd "%~dp0" >nul 2>&1 || (
  echo Failed to change to repository root.
  endlocal
  exit /b 1
)

IF EXIST "%CD%\build" (
  rmdir /s /q "%CD%\build"
)
mkdir "%CD%\build"
echo Recreated build directory: %CD%\build

popd >nul 2>&1
endlocal
tel