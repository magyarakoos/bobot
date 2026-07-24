#include <pico/stdlib.h>
#include <cstdio>
#include "bobot.h"

int main() {
    stdio_init_all();

    Bobot bobot;

    bobot.sc.drive(300, 300);

    while (true) {
        printf("%6.4f %6.4f %6.4f\n", bobot.sc.last_l_rpm, bobot.sc.last_r_rpm, bobot.ultra.dist());
        sleep_ms(100);
    }
}
