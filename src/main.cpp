#include <stdio.h>
#include "bobot.h"
#include "pico/stdlib.h"

int main() {
    Bobot::init();
    while (1) {
        sleep_ms(100);
    }
}
