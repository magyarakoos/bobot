// #include <pico/stdlib.h>
// #include <cstdio>
#include "onboard_led.h"
#include "pin.h"
#include "pwm.h"
// #include "pin.h"
// #include "config.h"
// #include "h_bridge.h"
// #include "servo.h"

int main() {
    stdio_init_all();

    Pin r(0), g(1), b(2);
    r.value(0);
    g.value(0);
    b.value(0);

    // PWM r(0), g(1), b(2);
    //
    // r.freq(2000);
    // g.freq(2000);
    // b.freq(2000);
    //
    // r.duty(0);
    // g.duty(0);
    // b.duty(0);

    OnboardLed led;
    while (1) {
        led.toggle();
        sleep_ms(200);
    }
    //
    // HBridge hb(HB_L1_PIN, HB_L2_PIN, HB_R1_PIN, HB_R2_PIN, HB_EEP_PIN, HB_ULT_PIN, HB_PWM_FREQ);
    // hb.drive(0, 0);
    //
    // PWM buzzer(BUZZER_PIN);
    // // buzzer.freq(1000);
    // // buzzer.duty(0.5);
    //
    // Servo servo(SERVO_PIN, SERVO_MIN, SERVO_MID, SERVO_MAX);
    // servo.deg(0);
    // // sleep_ms(1000);
    // // servo.deg(-90);
    // // sleep_ms(1000);
    // // servo.deg(90);
    // // sleep_ms(1000);
    // // servo.deg(0);
    //
    // Pin proxy(PROXY_PIN, GPIO_IN);
    // Pin proxy(3, GPIO_IN);
    //
    // while (true) {
    //     printf("%d\n", (int) proxy.value());
    //     sleep_ms(100);
    // }
}
