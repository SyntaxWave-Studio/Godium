#!/bin/bash
set -e
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE}")" && pwd)

echo "--- Building for Linux ---"
BUILD_DIR="$SCRIPT_DIR/build/linux"
mkdir -p "$BUILD_DIR"

cmake --fresh -S "$SCRIPT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR" -j$(nproc)

echo -e "\n--- Done! ---"
echo "Binary: $BUILD_DIR/Godium"
