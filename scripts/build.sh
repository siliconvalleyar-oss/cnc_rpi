#!/bin/bash
echo "Building CNC Controller with bcm2835..."
if [ ! -f "/usr/local/lib/libbcm2835.a" ]; then
    echo "bcm2835 library not found! Installing..."
    ./install_bcm2835.sh
fi
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
if [ $? -eq 0 ]; then
    echo "Build successful! Run with: sudo ./cnc_controller"
else
    echo "Build failed!"
fi
