#include "pid.h"
#include "debug.h"
#include "pico/stdlib.h"
#include "utils.h"

PID::PID(float _K_p, float _K_i, float _K_d, float _min_integral, float _max_integral)
    : K_p(_K_p),
      K_i(_K_i),
      K_d(_K_d),
      last_error(0),
      last_compute_t(0),
      integral(0),
      min_integral(_min_integral),
      max_integral(_max_integral),
      sp(0) {}

void PID::set_sp(float _sp) {
    sp = _sp;
}

void PID::set_constants(float _K_p, float _K_i, float _K_d, float _min_integral, float _max_integral) {
    K_p = _K_p;
    K_i = _K_i;
    K_d = _K_d;
    min_integral = _min_integral;
    max_integral = _max_integral;
}

float PID::compute(float pv) {
    float now = time_us_64() * 1e-6, dt = now - last_compute_t;

    float err = sp - pv;

    float p = K_p * err;

    integral += K_i * err * dt;
    integral = clamp(integral, min_integral, max_integral);

    float i = integral;

    float d = K_d * ((err - last_error) / dt);

    last_error = err;
    last_compute_t = now;

    return p + i + d;
}
