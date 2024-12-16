#include <stdio.h>
#include "bobot.h"
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    Bobot::init();

    // TODO: implement low frequency PWM for real this time
    PWM servo(16);
    servo.freq(50);
    servo.duty(0.1f);

    while (1);
}
