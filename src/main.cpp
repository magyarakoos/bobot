#include "bobot.h"
#include "h_bridge.h"
// #include "pin.h"
// #include "pico/stdlib.h"
// #include "pin.h"
// #include "h_bridge.h"

int main() {
    Bobot::init();
    Bobot::led.on();
    Pin r1(12), r2(13);
    r1.value(1);
    r2.value(1);
    while (1) {
        // if (Bobot::hb.ult.value()) {
        //     Bobot::led.off();
        // } else {
        //     Bobot::led.on();
        // }
        // sleep_ms(10);
    }
}
