#include <stdio.h>
#include "bobot.h"
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    Bobot::init();

    Bobot::servo.deg(0);
    sleep_ms(1000);
    Bobot::servo.deg(-90);
    sleep_ms(1000);
    Bobot::servo.deg(90);
    sleep_ms(1000);
    Bobot::servo.deg(0);

    while (1);
}
