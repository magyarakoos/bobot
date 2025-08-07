#pragma once

#include <cstdint>

// the variable names are based on this Wikipedia article:
// https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller#Control_function
class PID {
    volatile float K_p;
    volatile float K_i;
    volatile float K_d;

    float last_error;
    int64_t last_compute_t;

    float error(float target);

    volatile float integral;
    volatile float min_integral;
    volatile float max_integral;

    float P(float pv);
    float I(float pv, float dt);
    float D(float pv, float dt);

    // target value
    volatile float sp;

public:
    PID(float _K_p, float _K_i, float _K_d, float _min_integral, float _max_integral);

    void set_sp(float _sp);

    void set_constants(float _K_p, float _K_i, float _K_d, float _min_integral, float _max_integral);

    float compute(float pv);
};
