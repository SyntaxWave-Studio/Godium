#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
STATIC_DIR="$SCRIPT_DIR/build-static"
BINARY="$BUILD_DIR/Godium"

"$SCRIPT_DIR/build.sh"

rm -rf "$STATIC_DIR"
mkdir -p "$STATIC_DIR"

EXTRA_PLATFORM_PLUGINS="libqwayland.so" EXTRA_QT_MODULES="waylandcompositor" QMAKE=/usr/bin/qmake6 NO_STRIP=1 linuxdeploy --appdir "$STATIC_DIR" --plugin qt --executable "$BINARY"

echo "Static deploy completed: $STATIC_DIR/usr/bin/Godium"
