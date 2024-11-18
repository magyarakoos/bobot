#include "bobot.h"

int main() {
    Bobot::init();

    Bobot::buzzer.freq(2000);
    Bobot::buzzer.duty(0.9);

    Bobot::hb.drive(0.5, 0.5);

    while (true) {
        sleep_ms(1000);
    }
}
