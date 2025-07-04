#!/bin/bash

set -e

if [ ! -d "build" ]; then
    mkdir build
fi

cd build

cmake ..

make

picotool load -f bobot.uf2

# note: this is usually redundant, but occasionally
# the device wouldn't reboot on its own
# picotool reboot
