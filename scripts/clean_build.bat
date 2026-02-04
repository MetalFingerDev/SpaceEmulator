@echo off
REM Legacy wrapper: delegate to root `build.bat` helper
setlocal

pushd "%~dp0\.." >nul 2>&1 || (
  echo Failed to change to repository root.
  endlocal
  exit /b 1
)

IF EXIST "%CD%\build.bat" (
  call "%CD%\build.bat" %*
  popd >nul 2>&1
  endlocal
  exit /b %ERRORLEVEL%
)

echo This script has been moved to: %CD%\build.bat
popd >nul 2>&1
endlocal
exit /b 1
