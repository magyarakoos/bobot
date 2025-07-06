#include "encoder.h"
#include "bobot.h"
#include "pico/stdlib.h"
#include "utils.h"

Encoder::Encoder(uint _a, uint _b) : A(_a, GPIO_IN), B(_b, GPIO_IN), n(0), dir(1) {}

int Encoder::get_speed() {
    uint64_t now = time_us_64();

    // Bobot::print("%d\n", buffer.size());
    while (!buffer.empty() && buffer.front() + SPEED_WINDOW_US < now) {
        buffer.pop_front();
    }

    return buffer.size();
}

void Encoder::callback_a_rise() {
    uint64_t now = time_us_64();
    buffer.push_back(now);

    if (dir) {
        n++;
    } else {
        n--;
    }

    n %= 360;
}

void Encoder::callback_b_rise() {
    if (dir == 0) {
        // buffer.clear();
        dir = 1;
    }
}

void Encoder::callback_b_fall() {
    if (dir == 1) {
        // buffer.clear();
        dir = 0;
    }
}
