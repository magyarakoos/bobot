#pragma once

#include "pico/stdlib.h"
#include "pwm.h"

// motor controller
class HBridge {
    Pin l1;
    Pin l2;
    PWM l_pwm;

    Pin r1;
    Pin r2;
    PWM r_pwm;


    float last_l_speed;
    float last_r_speed;
public:
    float l_speed;
    float r_speed;

    HBridge(uint _l1, uint _l2, uint l_pwm, uint _r1, uint _r2, uint r_pwm, uint pwm_freq);

    void enable();
    void disable();

    // sets the motor PWMs to a speed between -1 and 1
    void drive(float l, float r);
};
