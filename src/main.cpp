#include <stdio.h>
#include "bobot.h"
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    Bobot::init();

    Bobot::hb.drive(0.5, 0.5);

    while (1);
}
