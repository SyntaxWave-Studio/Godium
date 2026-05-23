#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
BINARY="$BUILD_DIR/Godium"

"$SCRIPT_DIR/build.sh"

echo "========================================="
echo "Running Linux Binary: $BINARY"
echo "========================================="

exec "$BINARY" "$@"
