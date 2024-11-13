# Bobot firmware written in C++

## Setup (WIP)
- Download [pico-sdk](https://github.com/raspberrypi/pico-sdk)
- Set the environment variable `PICO_SDK_PATH` to the location of the SDK
- Plug in your **Bobot** in `BOOTSEL` mode
- Run `build.sh`

## Serial USB
- You can use `sudo screen <path>` to connect to the device and listen to stdout
- Path is usually `/dev/ttyACM0`