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

    float last_l_speed;
    float last_r_speed;

    // sets the motor PWMs to a speed between -1 and 1
    void drive_raw(float l, float r);

    static constexpr float STEP_SIZE = 0.1;
    static const uint DELAY_CYCLES = 10;

public:
    float l_speed;
    float r_speed;

    Pin ult;

    HBridge(uint _l1, uint _l2, uint _r1, uint _r2, uint _eep, uint _ult, uint pwm_freq);

    void enable();
    void disable();

    // sets the motor PWMs to a speed between -1 and 1
    // with a linear interpolation between the previous value and the next value
    void drive(float l, float r);
};
