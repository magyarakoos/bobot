#include "bobot.h"
#include <stdio.h>

namespace Bobot {

PWM buzzer = { BUZZER_PIN };
HBridge hb = { 10, 11, 12, 13, 14, 15, 2000 };
OnboardLed led;
Pin button = { BUTTON_PIN, GPIO_IN, true };
UltraSensor ultra = { ULTRA_TRIG_PIN, ULTRA_ECHO_PIN };
// ProximitySensor proxy = {};
RgbSensor rgb_sensor = {};
Servo servo = { 16, 0.032f, 0.075f, 0.13f };

struct repeating_timer ultra_trig_up_timer;
struct repeating_timer ultra_trig_down_timer;

// void proxy_handler() {}
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
    gpio_set_irq_enabled_with_callback(ultra.echo.pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_irq);
    add_repeating_timer_ms(-60, &ultra_trig_up, NULL, &ultra_trig_up_timer);
    sleep_us(15);
    add_repeating_timer_ms(-60, &ultra_trig_down, NULL, &ultra_trig_down_timer);

    gpio_set_irq_enabled_with_callback(button.pin, GPIO_IRQ_EDGE_FALL, true, &gpio_irq);
}

void enable() {
    if (bobot_enabled)
        return;

    bobot_enabled = true;

    buzzer.enable();
    ultra.enable();
}

void disable() {
    if (!bobot_enabled)
        return;

    bobot_enabled = false;

    buzzer.disable();
    ultra.disable();
}

void toggle() {
    if (bobot_enabled)
        disable();
    else
        enable();
}

// typedef void (*GpioIrq)();

// static GpioIrq irqs_rise[32] = { 0 };
// static GpioIrq irqs_fall[32] = { 0 };
void gpio_irq(uint gpio, uint32_t event_mask) {

    // GpioIrq irq = event_mask & GPIO_IRQ_EDGE_RISE ? irqs_rise[gpio] : irqs_fall[gpio];

    // if (irq != NULL)
    //     irq();

    // ultra sensor logic
    if (gpio == ultra.echo.pin) {
        if (event_mask & GPIO_IRQ_EDGE_RISE)
            ultra.rise = time_us_64();
        else if (event_mask & GPIO_IRQ_EDGE_FALL)
            ultra.fall = time_us_64();
    }

    // pause button logic
    else if (gpio == button.pin && event_mask & GPIO_IRQ_EDGE_FALL) {
        uint64_t now = time_us_64();

        if (now - last_pause_us >= DEBOUNCE_INTERVAL_US) {
            last_pause_us = now;
            toggle();
        }
    }

    // // ultra sensor logic
    // else if (gpio == proxy.pin)
    //     proxy.handler();
}
} // namespace Bobot
