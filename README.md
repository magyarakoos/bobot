# Bobot firmware written in C++

## Setup
- download the toolchain:
    - this can be done manually, but the simplest way is to download the [Pico](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico) extension for VSCode and create a project, this handles everything automatically
    - you can simply delete this project you had to create
- plug in your **Bobot** in `BOOTSEL` mode
- run `./build.sh`

## Serial USB
- you can use `sudo cat /dev/ttyACMX` to connect to the device and listen to stdout
- path is usually `/dev/ttyACM0`
- (you can use the VSCode Serial Monitor on Windows)
