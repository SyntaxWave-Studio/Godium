#!/bin/bash
set -e
PROJECT_ROOT=$(pwd)

echo "--- Building for Linux ---"
mkdir -p build/linux
cd build/linux
cmake ../.. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

#cd $PROJECT_ROOT

#echo -e "\n--- Building for Windows ---"
#mkdir -p build/windows
#cd build/windows
#x86_64-w64-mingw32-cmake ../.. -DCMAKE_BUILD_TYPE=Release
#make -j$(nproc)

echo -e "\n--- Done! ---"
echo "Linux binary: build/linux/Godium"
#echo "Windows binary: build/windows/Godium.exe"
