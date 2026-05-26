#!/bin/bash
set -e

if command -v pacman &> /dev/null; then
    sudo pacman -S --noconfirm qt6-base mesa
fi

echo "Done!"