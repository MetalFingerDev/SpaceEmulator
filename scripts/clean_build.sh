#!/usr/bin/env bash
# Remove and recreate build directory (POSIX / Git Bash)
set -e
if [ -d build ]; then
  rm -rf build
fi
mkdir -p build
echo "Recreated build directory: $(pwd)/build"
