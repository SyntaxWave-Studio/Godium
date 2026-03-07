#!/bin/bash
set -e
PROJECT_ROOT=$(pwd)

./build.sh

echo "Launching linux binary"
cd build/linux
exec ./GodiumIDE
