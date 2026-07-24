#pragma once

#include "pwm.h"

class HBridge {
public:
    HBridge(uint la_pin, uint lb_pin, uint ra_pin, uint rb_pin, uint freq = 2000);

    void drive(int l, int r);

private:
    Pwm la_pwm;
    Pwm lb_pwm;
    Pwm ra_pwm;
    Pwm rb_pwm;
};
