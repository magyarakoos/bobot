#include "speed_control.h"
#include <cstdio>

SpeedControl::SpeedControl(int hb_la_pin,
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
                           float int_max)

    : hb(hb_la_pin, hb_lb_pin, hb_ra_pin, hb_rb_pin),
      l_enc_(enc_la_pin, enc_lb_pin),
      r_enc_(enc_ra_pin, enc_rb_pin),
      l_pid_(kp, ki, kd, int_min, int_max, 0.0f),
      r_pid_(kp, ki, kd, int_min, int_max, 0.0f),
      l_rpm_sp_(0.0f),
      r_rpm_sp_(0.0f),
      last_l_ticks_(0),
      last_r_ticks_(0) {
    add_repeating_timer_ms(-(1000 / CONTROL_FREQ), &SpeedControl::control_loop_callback, this, &timer_);
}

bool SpeedControl::control_loop_callback(repeating_timer_t* rt) {
    static_cast<SpeedControl*>(rt->user_data)->control_loop();
    return true;
}

void SpeedControl::control_loop() {
    constexpr float dt = 1.0f / CONTROL_FREQ;

    l_pid_.sp = l_rpm_sp_;
    r_pid_.sp = r_rpm_sp_;

    int32_t current_l_ticks = l_enc_.ticks();
    int32_t current_r_ticks = r_enc_.ticks();

    int32_t delta_l_ticks = current_l_ticks - last_l_ticks_;
    int32_t delta_r_ticks = current_r_ticks - last_r_ticks_;

    last_l_ticks_ = current_l_ticks;
    last_r_ticks_ = current_r_ticks;

    last_l_rpm = static_cast<float>(delta_l_ticks) * CONTROL_FREQ * 60.0f / TICKS_PER_REV;
    last_r_rpm = static_cast<float>(delta_r_ticks) * CONTROL_FREQ * 60.0f / TICKS_PER_REV;

    int32_t l_effort = l_pid_.step(last_l_rpm, dt);
    int32_t r_effort = r_pid_.step(last_r_rpm, dt);

    last_l_effort = l_effort;
    last_r_effort = r_effort;

    hb.drive(l_effort, r_effort);
}

void SpeedControl::drive(float l_rpm, float r_rpm) {
    l_rpm_sp_ = l_rpm;
    r_rpm_sp_ = r_rpm;

    //hb.drive(l_rpm, r_rpm);
}
