#!/bin/bash
BINARY_NAME="Godium"

BUILD_DIR="$SCRIPT_DIR/build"
BUILD_TMP_DIR="$SCRIPT_DIR/build-tmp"
BUILD_STATIC_DIR="$SCRIPT_DIR/build-static"

BINARY="$BUILD_DIR/$BINARY_NAME"
BINARY_TMP="$BUILD_TMP_DIR/$BINARY_NAME"
BINARY_STATIC="$BUILD_STATIC_DIR/usr/bin/$BINARY_NAME"

# Linuxdeploy config
export EXTRA_PLATFORM_PLUGINS="libqwayland.so"
export EXTRA_QT_MODULES="waylandcompositor"
export QMAKE=/usr/bin/qmake6
export NO_STRIP=1
