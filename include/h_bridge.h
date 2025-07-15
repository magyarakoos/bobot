#pragma once

#include "pin.h"
#include "pwm.h"

// motor controller
class HBridge {
    PWM l1;
    PWM l2;

    PWM r1;
    PWM r2;

    Pin eep;

    uint pwm_freq;

    float last_l;
    float last_r;

    volatile bool inited;

public:
    float l_speed;
    float r_speed;

    Pin ult;

    HBridge(uint _l1, uint _l2, uint _r1, uint _r2, uint _eep, uint _ult, uint pwm_freq);

    void init();
    void deinit();

    // sets the motor PWMs to a speed between -1 and 1
    void drive(float l, float r);
};
