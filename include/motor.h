#pragma once

#include "encoder.h"
#include "h_bridge.h"
#include "pico/stdlib.h"
#include "pid.h"

#define FIRFILTER_TAP_NUM 21

struct FIRFilter {
    float history[FIRFILTER_TAP_NUM];
    unsigned int last_index;
};

static double filter_taps[FIRFILTER_TAP_NUM] = { -0.03853130909315966,  -0.08432805127919643,  -0.014345024558499378,
                                                 0.049610944740786825,  -0.028753002096226735, -0.033361432502414974,
                                                 0.07762703470336939,   -0.03132020759983342,  -0.1180019662091828,
                                                 0.28973140028931527,   0.6343813083073765,    0.28973140028931527,
                                                 -0.1180019662091828,   -0.03132020759983342,  0.07762703470336939,
                                                 -0.033361432502414974, -0.028753002096226735, 0.049610944740786825,
                                                 -0.014345024558499378, -0.08432805127919643,  -0.03853130909315966 };
// PID Motor controller
class Motor {
    volatile bool inited;

public:
    HBridge hb;

    Encoder enc_left;
    PID pid_left;

    Encoder enc_right;
    PID pid_right;

    volatile float l_target;
    volatile float r_target;

    FIRFilter fl, fr;

    Motor(uint hb_l1,
          uint hb_l2,
          uint hb_r1,
          uint hb_r2,
          uint hb_eep,
          uint hb_ult,
          uint hb_pwm_freq,
          uint enc_la,
          uint enc_lb,
          uint enc_ra,
          uint enc_rb,
          float pid_K_p,
          float pid_K_i,
          float pid_K_d,
          float pid_min_integral,
          float pid_max_integral);

    void init();
    void deinit();

    // Set motor target speed in RPM
    void drive(float l, float r);

    // This function should be called `MOTOR_PID_FREQ` times each second
    void timer_callback();
};
