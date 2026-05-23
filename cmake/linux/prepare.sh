#!/bin/bash
set -e

if command -v apt-get &> /dev/null; then
    sudo apt-get update
    sudo apt-get install -y qt6-base-dev libgl1-mesa-dev
elif command -v pacman &> /dev/null; then
    sudo pacman -S --noconfirm qt6-base mesa
elif command -v dnf &> /dev/null; then
    sudo dnf install -y qt6-qtbase-devel mesa-libGL-devel
fi

echo "Done!"