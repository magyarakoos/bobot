#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include <algorithm>
#include <iostream>

#include "onboard_led.h"
#include "pin.h"
#include "pwm.h"
#include "h_bridge.h"

const uint LED_DELAY_MS = 100;
const uint BUZZER_PIN = 4;

int main() {
    stdio_init_all();

    OnboardLed led;

    PWM pwm = {BUZZER_PIN};

    // pwm.freq(2000);
    // pwm.duty(0.9);

    HBridge hb = {
        10,
        11,
        12,
        13,
        14,
        15,
        2000
    };

    hb.drive(1, 1);

    while (true) {
        led.toggle();
        sleep_ms(LED_DELAY_MS);
        led.toggle();
        sleep_ms(LED_DELAY_MS);
    }
}
