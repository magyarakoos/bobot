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

    Bobot::hb.drive(0.5, 0.5);
    while (true) {
        Bobot::print("%d\n", (int) Bobot::proxy.value());
        sleep_ms(100);
    }
}
