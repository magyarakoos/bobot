#pragma once

#include "pico/stdlib.h"
#include "pwm.h"

// motor controller
class HBridge {
    PWM l1;
    PWM l2;

    PWM r1;
    PWM r2;

    // enables / disables the H-bridge
    Pin eep;

    float last_l_speed;
    float last_r_speed;
public:
    float l_speed;
    float r_speed;

    // fault pin
    Pin ult;

    HBridge(uint _l1, uint _l2, uint _r1, uint _r2, uint _eep, uint _ult, uint pwm_freq);

    void enable();
    void disable();

    // sets the motor PWMs to a speed between -1 and 1
    void drive(float l, float r);
};
