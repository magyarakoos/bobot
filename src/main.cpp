#include <stdio.h>
#include "bobot.h"
#include "pico/stdlib.h"

#include "pid.h"

int main() {
    stdio_init_all();
    Bobot::init();

    PID pid(0.3, 2, 0, 0, 0, 0);

    while (1) {
        float dist = Bobot::ultra.distance();

        if (dist == -1) dist = 0.3;

        float out = pid.compute(dist);
        Bobot::hb.drive(-out, -out);

        sleep_ms(60);
    }
}
