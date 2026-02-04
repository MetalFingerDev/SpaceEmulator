#!/usr/bin/env bash
set -euo pipefail

# Legacy wrapper: delegate to root `build` helper
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

if [ -x "$REPO_ROOT/build" ]; then
  exec "$REPO_ROOT/build" "$@"
else
  echo "This script has been moved to: $REPO_ROOT/build" >&2
  exit 1
fi
