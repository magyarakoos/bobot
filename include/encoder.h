#pragma once

#include "config.h"
#include "pin.h"
#include "ring_buffer.h"

class Encoder {
    volatile bool inited;

public:
    Pin A;
    Pin B;

    Encoder(uint _a, uint _b);

    // const uint TICKS_PER_REV = 360;

    // volatile int n;
    // volatile int dir;

    volatile int32_t n;
    volatile int32_t elapsed;

    volatile uint64_t last_irq;

    // RingBuffer<volatile uint64_t, ENC_BUFFER_SIZE> buffer;

    void init();
    void deinit();

    // gets the speed measured in RPM
    float get_speed();

    void callback_a_rise();
};
