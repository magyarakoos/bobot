#include "bobot.h"
#include <stdio.h>

namespace Bobot {

PWM buzzer = {BUZZER_PIN};
HBridge hb = {10, 11, 12, 13, 14, 15, 2000};
OnboardLed led;
Pin button = {BUTTON_PIN, GPIO_IN, true};
UltraSensor ultra = {ULTRA_TRIG_PIN, ULTRA_ECHO_PIN};

struct repeating_timer ultra_trig_up_timer;
struct repeating_timer ultra_trig_down_timer;

bool ultra_trig_up(__unused repeating_timer* t) {
    ultra.trig.value(1);
    ultra.last_pulse = time_us_64();
    return true;
}

bool ultra_trig_down(__unused repeating_timer* t) {
    ultra.trig.value(0);
    return true;
}

void init() {
    gpio_set_irq_enabled_with_callback(
        ultra.echo.pin,
        GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true,
        &gpio_irq
    );

    add_repeating_timer_ms(-60, &ultra_trig_up, NULL, &ultra_trig_up_timer);
    sleep_us(15);
    add_repeating_timer_ms(-60, &ultra_trig_down, NULL, &ultra_trig_down_timer);

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
    // ultra sensor logic
    if (gpio == ultra.echo.pin) {
        if (event_mask & GPIO_IRQ_EDGE_RISE) {
            ultra.rise = time_us_64();
        } else if (event_mask & GPIO_IRQ_EDGE_FALL) {
            ultra.fall = time_us_64();
        }
        return;
    }

    // pause button logic
    if (gpio == button.pin &&
        event_mask & GPIO_IRQ_EDGE_FALL) {
        uint64_t now = time_us_64();

        if (now - last_pause_us < DEBOUNCE_INTERVAL_US) {
            return;
        }

        last_pause_us = now;
        toggle();

        return;
    }
}
} // namespace Bobot