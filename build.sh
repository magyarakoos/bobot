#!/bin/bash

if [ ! -d "build" ]; then
    mkdir build
fi

cd build

cmake .. -DPICO_BOARD=pico_w

make

picotool load -f bobot.uf2
picotool reboot