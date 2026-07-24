#pragma once

#include <cstdint>

#include "pico/time.h"

#include "encoder.h"
#include "h_bridge.h"
#include "utils.h"

class SpeedControl {
public:
    SpeedControl(int hb_la_pin,
                 int hb_lb_pin,
                 int hb_ra_pin,
                 int hb_rb_pin,
                 int enc_la_pin,
                 int enc_lb_pin,
                 int enc_ra_pin,
                 int enc_rb_pin,
                 float kp,
                 float ki,
                 float kd,
                 float int_min,
                 float int_max);

    void drive(float l_rpm, float r_rpm);

    volatile float last_l_rpm;
    volatile float last_r_rpm;

private:
    static constexpr int CONTROL_FREQ = 50;
    static constexpr int TICKS_PER_REV = 1440;

    static bool control_loop_callback(repeating_timer_t* rt);
    void control_loop();

    HBridge hb;
    Encoder l_enc_;
    Encoder r_enc_;
    Pid l_pid_;
    Pid r_pid_;

    volatile float l_rpm_sp_;
    volatile float r_rpm_sp_;

    int32_t last_l_ticks_;
    int32_t last_r_ticks_;

    repeating_timer_t timer_;
};
