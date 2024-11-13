cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
set(PROJECT_NAME "bobot")

# needed for LSP support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# import the SDK directly
include(${PICO_SDK_PATH}/pico_sdk_import.cmake)

# initialize the Raspberry Pi Pico SDK
project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w CACHE STRING "Board type")

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(${PROJECT_NAME}
    main.cpp
)

# needed for serial USB communication
pico_enable_stdio_usb(${PROJECT_NAME} 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})

# add used pico libraries
target_link_libraries(${PROJECT_NAME} pico_stdlib pico_cyw43_arch_none)
