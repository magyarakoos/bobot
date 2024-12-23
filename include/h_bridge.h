#pragma once

#include "pico/stdlib.h"
#include "pwm.h"

// motor controller
class HBridge {
    PWM l1;
    PWM l2;

    PWM r1;
    PWM r2;

    // NOTE: these pins are currently unused
    // enables / disables the H-bridge
    Pin eep;
    // fault pin
    Pin ult;

    float l_speed;
    float r_speed;

public:
    HBridge(uint _l1, uint _l2, uint _r1, uint _r2, uint _eep, uint _ult, uint pwm_freq);

    // sets the motor PWMs to a speed between -1 and 1
    void drive(float l, float r);
};
