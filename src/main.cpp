#include <cstdio>
#include "bobot.h"

int main() {
    Bobot::init();
    Bobot::led.on();

    Bobot::rgb_sensor.led.value(1);
    while (true) {
        uint64_t now = time_us_64();
        Color c = Bobot::rgb_sensor.measure();
        printf("%d %llu\n", c.c, time_us_64() - now);
        // printf("%f\n", Bobot::ultra.distance());
        sleep_ms(50);
    }
    // OnboardLed led;
    // led.on();
    //
    // HBridge hb(HB_L1_PIN, HB_L2_PIN, HB_R1_PIN, HB_R2_PIN, HB_EEP_PIN, HB_ULT_PIN, HB_PWM_FREQ);
    // hb.drive(1, 1);
    //
    // PWM buzzer(BUZZER_PIN);
    // buzzer.freq(1000);
    // buzzer.duty(0.5);
    //
    // Servo servo(SERVO_PIN, SERVO_MIN, SERVO_MID, SERVO_MAX);
    // servo.deg(0);
    // sleep_ms(1000);
    // servo.deg(-90);
    // sleep_ms(1000);
    // servo.deg(90);
    // sleep_ms(1000);
    // servo.deg(0);
    //
    // Pin proxy(PROXY_PIN, GPIO_IN);
    //
    // sleep_ms(2000);
    //
    // hb.disable();
    // buzzer.disable();
    // proxy.disable();
    //
    // sleep_ms(2000);
    //
    // hb.enable();
    // buzzer.enable();
    // proxy.enable();
    //
    // sleep_ms(2000);
    //
    // hb.disable();
    // buzzer.disable();
    // proxy.disable();
    //
    // while (true) {
    //     sleep_ms(1000);
    // }
}
