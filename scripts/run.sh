#!/bin/bash
cd "$(dirname "$0")"
if [ -f "./build/cnc_controller" ]; then
    sudo ./build/cnc_controller
elif [ -f "./cnc_controller" ]; then
    sudo ./cnc_controller
else
    ./build.sh
    sudo ./build/cnc_controller
fi
