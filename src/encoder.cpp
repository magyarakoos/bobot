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

int Encoder::get_speed_tpw() {
    if (!inited)
        return 0;

    uint64_t now = time_us_64();

    while (!buffer.empty() && buffer.front() + ENC_BUFFER_TIME_WINDOW_US < now) {
        buffer.pop_front();
    }

    return buffer.size() * (dir ? 1 : -1);
}

float Encoder::get_speed() {
    // ticks / second (Hz)
    float tps = get_speed_tpw() * (1e6 / ENC_BUFFER_TIME_WINDOW_US);

    // revolutions / second (Hz)
    float rps = tps / TICKS_PER_REV;

    // wheel perimeter (m)
    const float wheel_perimeter_m = WHEEL_DIAMETER_MM * M_PI / 1000;

    // meters / second
    float mps = rps * wheel_perimeter_m;

    // centimeters / second
    return mps * 100;
}

void Encoder::callback_a_rise() {
    if (!inited)
        return;

    uint64_t now = time_us_64();
    buffer.push_back(now);

    if (B.get()) {
        dir = 1;
        n++;
    } else {
        dir = 0;
        n--;
    }
}
