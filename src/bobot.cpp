#include "bobot.h"
#include <stdio.h>

namespace Bobot {

    // Pin button = {BUTTON_PIN, GPIO_IN, true};
    // PWM buzzer = {BUZZER_PIN};
    HBridge hb = {10, 11, 12, 13, 14, 15, 2000};
    UltraSensor ultra = {20, 19};
    OnboardLed led;    

    struct repeating_timer trig_up_timer;
    struct repeating_timer trig_down_timer;

    uint64_t last_pulse;
    bool trig_up(__unused struct repeating_timer *t) {
        led.on();
        if (last_pulse > 0)
            ultra.distance = -1;

        ultra.trig.value(1);
        last_pulse = time_us_64();

        return true;
    }
    bool trig_down(__unused struct repeating_timer *t) {
        ultra.trig.value(0);
        return true;
    }


    void init() {

        // gpio_set_irq_enabled_with_callback(
        //     button.pin, 
        //     GPIO_IRQ_EDGE_FALL, 
        //     true, 
        //     &gpio_irq
        // );

        bool added_timer = add_repeating_timer_ms(-60, trig_up, NULL, &trig_up_timer);
        printf("added_timer: %s\n", added_timer ? "true" : "false");
        // sleep_us(15);
        // add_repeating_timer_ms(-60, trig_down, NULL, &trig_down_timer);

        // gpio_set_irq_enabled_with_callback(
        //     ultra.echo.pin,
        //     GPIO_IRQ_EDGE_RISE,
        //     true,
        //     &gpio_irq
        // );

        // gpio_set_irq_enabled_with_callback(
        //     ultra.echo.pin,
        //     GPIO_IRQ_EDGE_FALL,
        //     true,
        //     &gpio_irq
        // );
    }

    void enable() {
        if (bobot_enabled) return;
        bobot_enabled = true;

        buzzer.enable();
        hb.enable();
        ultra.enable();
    }

    void disable() {
        if (!bobot_enabled) return;
        bobot_enabled = false;

        buzzer.disable();
        hb.disable();
        ultra.disable();
    }

    void toggle() {
        bobot_enabled ? disable() : enable();
    }

    void gpio_irq(uint gpio, uint32_t event_mask) {
        // pause button logic
        if (gpio == button.pin && event_mask & GPIO_IRQ_EDGE_FALL) {
            uint64_t now = time_us_64();

            if (now - last_pause_us < DEBOUNCE_INTERVAL_US) {
                return;
            }

            last_pause_us = now;
            toggle();

            return;
        }

        // ultra sensor
        if (gpio == ultra.echo.pin) {
            uint64_t elapsed = time_us_64();
            ultra.distance = ultra.calculate_distance(elapsed);
            return;
        }
    }
}