#pragma once

#include <pico/types.h>

class Pwm {
public:
    Pwm(uint gpio, uint32_t freq_hz);

    void freq(uint32_t freq_hz);
    void duty_u16(uint16_t duty);

private:
    void apply_freq(uint32_t freq_hz);

    uint gpio_;
    uint slice_;
    uint channel_;
    uint32_t wrap_;
};
