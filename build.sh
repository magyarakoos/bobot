#!/bin/bash

set -e

cmake . -B build
cmake --build build

sudo ./picotool load -F build/bobot.uf2
sudo ./picotool reboot
