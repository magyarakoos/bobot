#!/bin/bash

if [ ! -d "build" ]; then
    mkdir build
fi

cd build

cmake .. -DPICO_BOARD=pico_w

make

picotool load -f bobot.uf2

# note: this is usually redundant, but occasionally
# the device wouldn't reboot on its own
# picotool reboot
