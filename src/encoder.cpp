#include "encoder.h"
#include "bobot.h"
#include "config.h"
#include "pico/stdlib.h"
#include "utils.h"

#define M_PI 3.1415926535

Encoder::Encoder(uint _a, uint _b) : inited(false), A(_a, GPIO_IN), B(_b, GPIO_IN), n(0), dir(1) {}

void Encoder::init() {
    if (inited)
        return;

    inited = true;

    A.init();
    B.init();
}

void Encoder::deinit() {
    if (!inited)
        return;

    inited = false;

    A.deinit();
    B.deinit();
}

float Encoder::get_speed() {
    if (!inited)
        return 0;

    uint64_t now = time_us_64();

    while (!buffer.empty() && buffer.front() + ENC_BUFFER_TIME_WINDOW_US < now) {
        buffer.pop_front();
    }

    // ticks per `TIME_WINDOW_US`
    float speed_tpw = buffer.size() * dir;

    // ticks per second (Hz)
    float speed_tps = speed_tpw * (1e6 / ENC_BUFFER_TIME_WINDOW_US);

    // ticks per minute
    float speed_tpm = speed_tps * 60;

    // revolutions per minute
    float speed_rpm = (float) speed_tpm / TICKS_PER_REV;

    return speed_rpm;
}

void Encoder::callback_a_rise() {
    if (!inited)
        return;

    uint64_t now = time_us_64();
    buffer.push_back(now);

    dir = (B.get() ? 1 : -1);
    n += dir;
}
