#pragma once

#include <cstdint>

#include "hardware/pio.h"

class Encoder {
public:
    explicit Encoder(uint a_pin, uint b_pin, PIO pio = pio0, int max_step_rate = 0);

    int32_t ticks() const;

private:
    PIO pio_;
    uint sm_;
};
