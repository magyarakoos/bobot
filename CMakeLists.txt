# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME "bobot")
set(PICO_BOARD pico2_w)

# needed for LSP support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# include the SDK directly
include(pico_sdk_import.cmake)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

# initialize pico-sdk
project(${PROJECT_NAME} C CXX ASM)
pico_sdk_init()

# add cpp files
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/pin.cpp
    src/pwm.cpp
    src/buzzer.cpp
    src/rgb_led.cpp
    src/ultra_sensor.cpp
    src/bobot.cpp
    src/gpio_irq.cpp
    src/servo.cpp
    src/encoder.cpp
    src/h_bridge.cpp
    src/speed_control.cpp
    src/utils.cpp
)

# needed for serial USB communication
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# create the uf2 file
pico_add_extra_outputs(${PROJECT_NAME})

# needed for the PIO encoder
pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/encoder.pio)

# include headers
target_include_directories(${PROJECT_NAME} PRIVATE include)

# pull in pico libraries that we need
target_link_libraries(
    ${PROJECT_NAME}
    pico_stdlib
    hardware_gpio
    hardware_pwm
    hardware_clocks
    hardware_pio
)

pico_set_program_name(${PROJECT_NAME} "bobot-drivers")
pico_set_program_description(${PROJECT_NAME} "bobot driver implementations, written in C++")
pico_set_program_url(${PROJECT_NAME} "https://github.com/magyarakoos/bobot")
