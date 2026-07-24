#pragma once

#include "pwm.h"

class Servo {
public:
    Servo(uint pin, uint _min, uint _mid, uint _max);

    void deg(int d);

private:
    Pwm pwm;

    uint min;
    uint mid;
    uint max;

    void duty(uint16_t d);
};
