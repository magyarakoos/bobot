#pragma once

#include <stdint.h>

enum Channel : uint8_t { A = 0b01, B = 0b10 };

class Encoder {
private:
    uint8_t chanState;

public:
    Encoder();

    const int TICKS_PER_REV = 360;
    const int STOPPING_TRESHOLD_US = 1e7 / TICKS_PER_REV;

    volatile int n;
    volatile bool dir;

    volatile uint64_t last_revolution, dt_sum;

    float get_rps();

    static void callback(volatile Encoder& e, Channel chan);
};
