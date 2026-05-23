#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
STATIC_DIR="$SCRIPT_DIR/build-static"

if [ "$1" = "static" ]; then
    "$SCRIPT_DIR/build-static.sh"
    shift
    
    export QT_NO_GLOBAL_PLUGINS=1
    BINARY="$STATIC_DIR/usr/bin/Godium"
else
    "$SCRIPT_DIR/build.sh"
    BINARY="$BUILD_DIR/Godium"
fi

echo "========================================="
echo "Running Linux Binary: $BINARY"
echo "========================================="

exec "$BINARY" "$@"
