#include "bobot.h"

namespace Bobot {

PWM buzzer = { BUZZER_PIN };
HBridge hb = { 10, 11, 12, 13, 14, 15, 2000 };
OnboardLed led;
Pin button = { BUTTON_PIN, GPIO_IN, true };
UltraSensor ultra = { ULTRA_TRIG_PIN, ULTRA_ECHO_PIN };
RgbSensor rgb_sensor = {};
Servo servo = { 16, 0.032f, 0.075f, 0.13f };

float last_hb_l = 0, last_hb_r = 0;

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
    add_irq(ultra.echo.pin, false, [&]() { ultra.rise = time_us_64(); });
    add_irq(ultra.echo.pin, true, [&]() { ultra.fall = time_us_64(); });

    add_repeating_timer_ms(-60, &ultra_trig_up, NULL, &ultra_trig_up_timer);
    sleep_us(15);
    add_repeating_timer_ms(-60, &ultra_trig_down, NULL, &ultra_trig_down_timer);

    add_irq(button.pin, true, [&]() {
        uint64_t now = time_us_64();
        if (now - last_pause_us >= DEBOUNCE_INTERVAL_US) {
            last_pause_us = now;
            toggle();
        }
    });

    servo.deg(0);
}

void enable() {
    if (bobot_enabled)
        return;

    bobot_enabled = true;

    hb.enable();
    buzzer.enable();
    ultra.enable();
}

void disable() {
    if (!bobot_enabled)
        return;

    bobot_enabled = false;

    hb.disable();
    buzzer.disable();
    ultra.disable();
}

void toggle() {
    (bobot_enabled ? disable : enable)();
}

// the first row is for edge rise, the second row is for edge fall
// the i-th function in each row is for the i-th gpio pin
static GpioIrq irqs[2][32] = { 0 };

void add_irq(uint gpio, bool is_fall, GpioIrq callback) {
    irqs[is_fall][gpio] = callback;

    uint32_t event_mask = is_fall ? GPIO_IRQ_EDGE_FALL : GPIO_IRQ_EDGE_RISE;
    gpio_set_irq_enabled_with_callback(gpio, event_mask, true, &gpio_irq);
}

void gpio_irq(uint gpio, uint32_t event_mask) {

    bool is_fall = event_mask & GPIO_IRQ_EDGE_FALL;
    GpioIrq irq = irqs[is_fall][gpio];

    if (irq)
        irq();
}
} // namespace Bobot
