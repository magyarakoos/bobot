#include "bobot.h"
// #include "onboard_led.h"

int main() {
    // OnboardLed led;
    // led.on();
    Bobot::init();
    Bobot::led.on();
    Bobot::hb.drive(-1, -1);
    // HBridge hb(10, 11, 8, 9, 14, 15, 2000);
    // hb.drive(1, 1);
    // sleep_ms(1000);
    // Bobot::led.off();
    // hb.drive(0.3, 0.3);
    while (1) {
        // if (hb.ult.value()) {
        //     Bobot::led.on(); 
        // } else {
        //     Bobot::led.off();
        // }
        // sleep_ms(1000);
    }
}
