#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE}")" && pwd)"
source "$SCRIPT_DIR/config.sh"

mkdir -p "$BUILD_TMP_DIR"

cmake -S "$SCRIPT_DIR" -B "$BUILD_TMP_DIR" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_TMP_DIR" --config Release --parallel

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cp "$BINARY_TMP" "$BINARY"

echo "Build completed: $BINARY"

if [ "$1" = "static" ]; then
    rm -rf "$BUILD_STATIC_DIR"
    mkdir -p "$BUILD_STATIC_DIR"

    linuxdeploy --appdir "$BUILD_STATIC_DIR" --plugin qt --executable "$BINARY"

    echo "Static deploy completed: $BINARY_STATIC"
fi
