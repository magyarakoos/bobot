#include <pico/stdlib.h>
#include <cmath>
#include <cstdio>
#include "h_bridge.h"
#include "pwm.h"

#define ULT_PIN 4
#define IN1_PIN 3
#define IN2_PIN 2
#define IN3_PIN 1
#define IN4_PIN 0
#define EEP_PIN 5

int main() {
    stdio_init_all();

    HBridge hb(IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN, EEP_PIN, ULT_PIN, 2000);

    hb.drive(1, 1);

    sleep_ms(1000);

    hb.drive(-1, -1);
    sleep_ms(1000);

    hb.disable();

    sleep_ms(1000);

    hb.enable();

    // gpio_init(EEP_PIN);
    // gpio_set_dir(EEP_PIN, GPIO_OUT);
    //
    // gpio_put(EEP_PIN, 1);
    //
    // gpio_init(ULT_PIN);
    // gpio_set_dir(ULT_PIN, GPIO_IN);
    // gpio_pull_up(ULT_PIN);
    //
    // // gpio_init(IN1_PIN);
    // // gpio_set_dir(IN1_PIN, GPIO_OUT);
    //
    // gpio_init(IN2_PIN);
    // gpio_set_dir(IN2_PIN, GPIO_OUT);
    //
    // // gpio_init(IN3_PIN);
    // // gpio_set_dir(IN3_PIN, GPIO_OUT);
    //
    // gpio_init(IN4_PIN);
    // gpio_set_dir(IN4_PIN, GPIO_OUT);
    //
    // // gpio_put(IN1_PIN, 0);
    // gpio_put(IN2_PIN, 0);
    // // gpio_put(IN3_PIN, 1);
    // gpio_put(IN4_PIN, 0);
    //
    // PWM in1(IN1_PIN), in3(IN3_PIN);
    //
    // in1.freq(2000);
    // in3.freq(2000);
    //
    // for (int i = 0; i <= 10; i++) {
    //     in1.duty((float) i / 10);
    //     in3.duty((float) i / 10);
    //     sleep_ms(100);
    //     // for (volatile int j = 0; j < 10; j++)
    //     //     ;
    // }

    // while (true) {
    //     // for (int i = 0; i < 10; i++) {
    //     //     in3.duty((float) i / 10);
    //     //     sleep_ms(200);
    //     // }
    //     // for (int i = 10; i >= 0; i--) {
    //     //     in3.duty((float) i / 10);
    //     //     sleep_ms(200);
    //     // }
    // }

    while (true) {
        printf("ULT:%d, L:%.2f, R:%.2f\n", (int) hb.ult.value(), hb.l_speed, hb.r_speed);
        sleep_ms(1000);
    }
}
