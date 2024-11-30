#include <stdio.h>
#include "pico/stdlib.h"
#include "bobot.h"

int main() {
    stdio_init_all();
    Bobot::init();

    Bobot::hb.drive(0.5, 0.5);

    while (true);
}