#include <stdio.h>
#include "bobot.h"
#include "pico/stdlib.h"

#include "pid.h"

#include <iostream>

// void pid_distancing() {
//     PID pid(0.3, 2, 0, 0, 0, 0);
//     while (1) {
//         float dist = Bobot::ultra.distance();

//         if (dist == -1) dist = 0.3;

//         float out = pid.compute(dist);
//         Bobot::hb.drive(-out, -out);

//         sleep_ms(60);
//     }
// }

int main() {
    stdio_init_all();
    Bobot::init();

    Bobot::rgb_sensor.led.value(1);

    while (1) {
        std::array<uint16_t, 4> values = Bobot::rgb_sensor.get_data();

        std::cout << values[0] << " " << values[1] << " " << values[2] << " " << values[3] << "\n";
        sleep_ms(100);
    }
}
