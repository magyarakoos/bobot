#include <stdio.h>
#include "bobot.h"
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    Bobot::init();

    while (1) {
        printf("%f\n", Bobot::ultra.distance());
        sleep_ms(60);
    }
}
