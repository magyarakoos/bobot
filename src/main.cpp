#include "bobot.h"
#include "pico/stdlib.h"

void servo_test() {
    Bobot::init();

    int pos = 0, dir = 1;
    while (true) {
        Bobot::servo.deg(pos);
        pos += dir;
        sleep_ms(5);
        Bobot::print("%d %d\n", pos, (int) dir);
        if (pos <= -90) {
            dir = 1;
        } else if (pos >= 90) {
            dir = -1;
        }
    }
}

int main() {
    Bobot::init();
    Bobot::led.on();
    // Bobot::hb.drive(1, 1);
    while (true) {
        Bobot::print("%d\n", Bobot::proxy.value());
        sleep_ms(50);
        // Bobot::print("%.4f %d\n", Bobot::enc_left.get_speed(), Bobot::enc_left.get_speed_tpw(),
        //              Bobot::enc_left.get_speed_tpw());
        // sleep_ms(200);
    }
}

// #include <stdio.h>
// #include "pico/stdlib.h"
//
// int main() {
//     stdio_init_all();
//     while (true) {
//         printf("Hello, World!\n");
//         sleep_ms(100);
//     }
// }
