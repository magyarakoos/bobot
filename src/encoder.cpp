#include "encoder.h"
#include "bobot.h"
#include "pico/stdlib.h"
#include "utils.h"

Encoder::Encoder(uint _a, uint _b) : A(_a, GPIO_IN), B(_b, GPIO_IN), n(0), last_revolution(0), dt_sum(0) {}

float Encoder::get_rps() {
    if (last_revolution == 0)
        return 0;

    float dt_avg = (float) dt_sum / n;
    // float dt = time_us_64() - last_revolution;
    float rps = 1e6 / TICKS_PER_REV / dt_avg;

    return rps;
}

void Encoder::callback_a_rise() {
    if (dir) {
        n++;
    } else {
        n--;
    }

    if (abs(n) >= 360) {
        last_revolution = time_us_64();
        n %= 360;
    }
}

void Encoder::callback_b_rise() {
    dir = 1;
}

void Encoder::callback_b_fall() {
    dir = 0;
}
