#pragma once

#include <cstdint>
#include "pico/stdlib.h"
#include "pwm.h"

class Servo {
private:
    PWM pwm;
    uint16_t min, mid, max;

public:
    Servo(uint pin, uint16_t min, uint16_t mid, uint16_t max);

    void duty(float d);

    void deg(float d);
    void rad(float r);
};
