#include "motor.h"
#include <cstdio>

void FIRFilter_init(FIRFilter* f) {
    for (int i = 0; i < FIRFILTER_TAP_NUM; ++i)
        f->history[i] = 0;
    f->last_index = 0;
}

void FIRFilter_put(FIRFilter* f, float input) {
    f->history[f->last_index++] = input;
    if (f->last_index == FIRFILTER_TAP_NUM)
        f->last_index = 0;
}

float FIRFilter_get(FIRFilter* f) {
    float acc = 0;
    int index = f->last_index;
    for (int i = 0; i < FIRFILTER_TAP_NUM; ++i) {
        index = index != 0 ? index - 1 : FIRFILTER_TAP_NUM - 1;
        acc += f->history[index] * filter_taps[i];
    }
    return acc;
}

Motor::Motor(uint hb_l1,
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
             float pid_max_integral)
    : inited(false),
      hb(hb_l1, hb_l2, hb_r1, hb_r2, hb_eep, hb_ult, hb_pwm_freq),
      enc_left(enc_la, enc_lb),
      pid_left(pid_K_p, pid_K_i, pid_K_d, pid_min_integral, pid_max_integral),
      enc_right(enc_ra, enc_rb),
      pid_right(pid_K_p, pid_K_i, pid_K_d, pid_min_integral, pid_max_integral),
      l_target(0),
      r_target(0) {}

void Motor::init() {
    if (inited)
        return;

    inited = true;

    hb.init();
    enc_left.init();
    enc_right.init();
    FIRFilter_init(&fl);
    FIRFilter_init(&fr);
}

void Motor::deinit() {
    if (!inited)
        return;

    inited = false;

    // we don't deinit the HBridge, only put it to sleep
    // the interface is a bit misleading this way, might refactor later
    // hb.deinit();
    hb.eep.set(0);

    enc_left.deinit();
    enc_right.deinit();
}

void Motor::drive(float l, float r) {
    if (!inited)
        return;

    l_target = l;
    r_target = r;
    // l_target = abs(l) >= 5 ? l : 0;
    // r_target = abs(r) >= 5 ? r : 0;

    pid_left.set_sp(l_target);
    pid_right.set_sp(r_target);
}

void Motor::timer_callback() {
    if (!inited)
        return;

    float l_speed = enc_left.get_speed() / 200.0f;
    float r_speed = enc_right.get_speed() / 200.0f;

    // FIRFilter_put(&fl, l_speed);
    // float lf = FIRFilter_get(&fl);
    // FIRFilter_put(&fr, r_speed);
    // float rf = FIRFilter_get(&fr);

    float l_o;
    if (l_target != 0) {
        l_o = pid_left.compute(l_speed);
        // l_o = max(10.f, abs(l_o)) * sign(l_o);
    } else {
        l_o = 0;
    }

    float r_o;
    if (r_target != 0) {
        r_o = pid_right.compute(r_speed);
        // r_o = max(10.f, abs(r_o)) * sign(r_o);
    } else {
        r_o = 0;
    }

    // printf("%f %f => %f %f\n", l_speed, r_speed, l_o, r_o);

    hb.drive(l_o, r_o);
}
