cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(pico_sdk_import.cmake)
project(bobot C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(bobot
    main.cpp
)

pico_enable_stdio_usb(bobot 1)

pico_add_extra_outputs(bobot)

target_link_libraries(bobot pico_stdlib pico_cyw43_arch_none)

