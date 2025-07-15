#pragma once

#include "pwm.h"

class Servo {
private:
    PWM pwm;
    float min, mid, max;

    volatile bool inited;

public:
    Servo(uint pin, float _min, float _mid, float _max);

    void init();
    void deinit();

    void duty(float d);

    void deg(float d);
    void rad(float r);
};
