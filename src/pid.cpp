#include "pid.h"
#include "pico/stdlib.h"

float PID::error(float pv) {
    return sp - pv;
}

float PID::P(float pv) {
    return K_p * error(pv);
}

float PID::I(float pv, int64_t dt) {
    return K_i * (integral += error(pv) * dt);
}

float PID::D(float pv, int64_t dt) {
    return K_d * ((error(pv) - last_error) / dt);
}

PID::PID(float _sp, float _K_p, float _K_i, float _K_d, float _min_integral, float _max_integral)
    : last_error(0),
      last_compute_t(0),
      sp(_sp),
      K_p(_K_p),
      K_i(_K_i),
      K_d(_K_d),
      min_integral(_min_integral),
      max_integral(_max_integral) {}

float PID::compute(float pv) {
    int64_t now = time_us_64(), dt = now - last_compute_t;

    float result = P(pv) + I(pv, dt) + D(pv, dt);

    last_error = error(pv);
    last_compute_t = now;

    return result;
}
