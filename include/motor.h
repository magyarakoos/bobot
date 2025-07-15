#pragma once

#include "encoder.h"
#include "h_bridge.h"
#include "pico/stdlib.h"
#include "pid.h"

// PID Motor controller
class Motor {
    volatile bool inited;

public:
    HBridge hb;

    Encoder enc_left;
    PID pid_left;

    Encoder enc_right;
    PID pid_right;

    int l_target;
    int r_target;

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

    // Set motor speed in TODO: system of measurement
    void drive(int l, int r);

    // This function should be called `MOTOR_PID_FREQ` times each second
    void timer_callback();
};
