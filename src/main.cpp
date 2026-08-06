#include <pico/stdlib.h>
#include <string>
#include "config.h"
#include "bobot.h"

int main() {
    Bobot bobot;
    bobot.rgb_sensor.led.on();
    while (true) {
        auto [c, r, g, b] = bobot.rgb_sensor.measure();
        printf("%d %d %d %d\n", c, r, g, b);
        sleep_ms(config::RGB_SENSOR_INTEGRATION_TIME_MS);
    }
}
