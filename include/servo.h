#pragma once

#include "pwm.h"

class Servo {
private:
    PWM pwm;
    float min, mid, max;

public:
    Servo(uint pin, float _min, float _mid, float _max);

    void enable();
    void disable();

    void duty(float d);

    void deg(float d);
    void rad(float r);
};
