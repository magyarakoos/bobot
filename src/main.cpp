#include "bobot.h"

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

    // Bobot::hb.drive(0.5, 0.5);
    while (true) {
        Bobot::print("%d %.4f\n", Bobot::enc_left.n, Bobot::enc_left.get_rps());
        sleep_ms(100);
        // if (rb.value()) {
        //     Bobot::print("ON\n");
        // } else {
        //     Bobot::print("OFF\n");
        // }
        // sleep_ms(10);
        // Bobot::print("%d\n", (int) Bobot::proxy.value());
        // sleep_ms(100);
    }
}
