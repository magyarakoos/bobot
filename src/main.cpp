#include <pico/stdlib.h>
#include <cstdio>
#include "bobot.h"

int main() {
    stdio_init_all();

    Bobot bobot;

    while (true) {
        auto [c, r, g, b] = bobot.rgb_sensor.measure();
        printf("%d %d %d %d\n", c, r, g, b);
        sleep_ms(100);
    }
}
