#!/usr/bin/env bash
set -euo pipefail

# Portable build-and-run helper
# Prefers MinGW (mingw32-make + g++), detects MSVC (cl), or falls back to a generic cmake build.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$SCRIPT_DIR"
BUILD_DIR="$REPO_ROOT/build"

echo "Repo root: $REPO_ROOT"
echo "Build dir: $BUILD_DIR"

if [ -d "$BUILD_DIR" ]; then
  echo "Removing existing build directory: $BUILD_DIR"
  rm -rf "$BUILD_DIR"
fi
mkdir -p "$BUILD_DIR"

# Use MinGW if available
if command -v mingw32-make >/dev/null 2>&1 && command -v g++ >/dev/null 2>&1; then
  GENERATOR="MinGW Makefiles"
  echo "Detected MinGW toolchain; using generator: $GENERATOR"
  cmake -G "$GENERATOR" -S "$REPO_ROOT" -B "$BUILD_DIR"
  JOBS="$(nproc 2>/dev/null || echo 1)"
  mingw32-make -C "$BUILD_DIR" -j"$JOBS"

# Use MSVC if available (cl)
elif command -v cl >/dev/null 2>&1; then
  echo "Detected MSVC (cl). Configuring for x64 using default generator."
  cmake -S "$REPO_ROOT" -B "$BUILD_DIR" -A x64
  cmake --build "$BUILD_DIR" --config Release -- /m

# Fallback to plain cmake
else
  echo "No MinGW or MSVC detected. Trying a generic CMake configure + build (may fail if no compiler installed)."
  cmake -S "$REPO_ROOT" -B "$BUILD_DIR"
  JOBS="$(nproc 2>/dev/null || echo 1)"
  cmake --build "$BUILD_DIR" -- -j"$JOBS"
fi

# Locate executable
EXE_NAME="PhysicsEngine"
POSSIBLE=("$BUILD_DIR/$EXE_NAME.exe" "$BUILD_DIR/Release/$EXE_NAME.exe" "$BUILD_DIR/Debug/$EXE_NAME.exe")
FOUND=""
for p in "${POSSIBLE[@]}"; do
  if [ -f "$p" ]; then
    FOUND="$p"
    break
  fi
done

if [ -z "$FOUND" ]; then
  # Fallback: find any exe in build
  FOUND="$(find "$BUILD_DIR" -maxdepth 3 -type f -iname "*.exe" -print -quit || true)"
fi

if [ -z "$FOUND" ]; then
  echo "Build completed but no executable was found in $BUILD_DIR." >&2
  echo "Please ensure a compiler is installed (e.g., MSYS2/Mingw-w64 or MSVC) and re-run this script." >&2
  exit 2
fi

echo "Found executable: $FOUND"

echo "Running executable..."
"$FOUND"
EXIT_CODE=$?
echo "Executable exited with code: $EXIT_CODE"
exit $EXIT_CODE
