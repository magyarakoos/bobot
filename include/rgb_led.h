#pragma once

#include "pwm.h"

class RGBLed {
public:
    RGBLed(uint r_gpio, uint g_gpio, uint b_gpio, uint32_t freq_hz = 2000);

    void set_color(uint8_t r, uint8_t g, uint8_t b);

private:
    Pwm r_;
    Pwm g_;
    Pwm b_;
};
