#include "bobot.h"

int main() {
    Bobot bobot;
    while (true) {
        bobot.led.on();
        sleep_ms(500);
        bobot.led.off();
        sleep_ms(500);
    }
}
