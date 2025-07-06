#pragma once

#include "config.h"
#include "pico/stdlib.h"
#include "pin.h"
#include "ring_buffer.h"

class Encoder {
public:
    Pin A;
    Pin B;

    Encoder(uint _a, uint _b);

    const int TICKS_PER_REV = 360;
    const int SPEED_WINDOW_US = 1e5;

    volatile int n;
    volatile bool dir;

    RingBuffer<volatile uint64_t, ENC_BUFFER_SIZE> buffer;

    // gets the speed measured in ticks / `SPEED_WINDOW_US`
    int get_speed();

    void callback_a_rise();
    void callback_b_rise();
    void callback_b_fall();
};
