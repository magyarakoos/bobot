#pragma once

#include "config.h"
#include "pico/stdlib.h"
#include "pin.h"
#include "ring_buffer.h"

class Encoder {
    volatile bool inited;

public:
    Pin A;
    Pin B;

    Encoder(uint _a, uint _b);

    const uint TICKS_PER_REV = 360;

    volatile int n;
    volatile bool dir;

    RingBuffer<volatile uint64_t, ENC_BUFFER_SIZE> buffer;

    void init();
    void deinit();

    // gets the speed measured in ticks / `ENC_SPEED_WINDOW_US`
    int get_speed_tpw();

    // gets the speed measured in cm/s
    float get_speed();

    void callback_a_rise();
};
