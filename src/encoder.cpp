#include "encoder.h"
#include "bobot.h"
#include "config.h"
#include "pico/stdlib.h"
#include "utils.h"

#define M_PI 3.1415926535

Encoder::Encoder(uint _a, uint _b) : A(_a, GPIO_IN), B(_b, GPIO_IN), n(0), dir(1) {}

int Encoder::get_speed_tpw() {
    uint64_t now = time_us_64();

    while (!buffer.empty() && buffer.front() + SPEED_WINDOW_US < now) {
        buffer.pop_front();
    }

    return buffer.size();
}

float Encoder::get_speed() {
    // ticks / second (Hz)
    float tps = get_speed_tpw() * (1e6 / SPEED_WINDOW_US);

    // revolutions / second (Hz)
    float rps = tps / TICKS_PER_REV;

    // wheel perimeter (m)
    const float wheel_perimeter_m = WHEEL_DIAMETER_MM * M_PI / 1000;

    // meters / second
    return rps * wheel_perimeter_m;
}

void Encoder::callback_a_rise() {
    uint64_t now = time_us_64();
    buffer.push_back(now);

    if (B.value()) {
        dir = 1;
        n++;
    } else {
        dir = 0;
        n--;
    }
}
