cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
set(PROJECT_NAME "bobot")

# needed for LSP support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# import the SDK directly
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

# initialize the Raspberry Pi Pico SDK
project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w CACHE STRING "Board type")

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(${PROJECT_NAME}
    main.cpp
    src/pin.cpp
    src/pwm.cpp
    src/onboard_led.cpp
    src/h_bridge.cpp
    src/ultra_sensor.cpp
    src/bobot.cpp
)

# needed for serial USB communication
pico_enable_stdio_usb(${PROJECT_NAME} 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})

# include directories for headers
target_include_directories(${PROJECT_NAME} PRIVATE include)

# pull in pico libraries that we need
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    pico_cyw43_arch_none
    hardware_pwm
    hardware_clocks)
