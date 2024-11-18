#include "bobot.h"

namespace Bobot {

    Pin button = {BUTTON_PIN, GPIO_IN, true};
    PWM buzzer = {BUZZER_PIN};
    HBridge hb = {10, 11, 12, 13, 14, 15, 2000};

    void init() {
        stdio_init_all();

        gpio_set_irq_enabled_with_callback(
            button.pin, 
            GPIO_IRQ_EDGE_FALL, 
            true, 
            &gpio_irq
        );
    }

    void enable() {
        if (bobot_enabled) return;
        bobot_enabled = true;

        buzzer.enable();
        hb.enable();
    }

    void disable() {
        if (!bobot_enabled) return;
        bobot_enabled = false;

        buzzer.disable();
        hb.disable();
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
        }
    }
}