#!/bin/bash

set -e

cmake . -B build
cmake --build build

sudo picotool load -ufx build/bobot.uf2
