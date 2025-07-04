#pragma once

#include "pwm.h"

// Common anode RGB LED controller
class RGBLed {
    PWM r;
    PWM g;
    PWM b;

    RGBLed(uint _r, uint _g, uint _b, uint freq);

    void set_color(float _r, float _g, float _b);
};
