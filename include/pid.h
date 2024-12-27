#pragma once

#include <stdint.h>

// the variable names are based on this Wikipedia article:
// https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller#Control_function
class PID {
    float K_p;
    float K_i;
    float K_d;

    float last_error;
    int64_t last_compute_t;

    float error(float target);

    float integral;
    float min_integral;
    float max_integral;

    float P(float pv);
    float I(float pv, int64_t dt);
    float D(float pv, int64_t dt);

public:
    float sp;

    PID(float _sp, float _K_p, float _K_i, float _K_d, float _min_integral, float _max_integral);

    float compute(float pv);
};
