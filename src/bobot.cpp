#include "bobot.h"
#include <pico/cyw43_arch.h>
#include <cstdio>
#include "config.h"

namespace Bobot {

OnboardLed led;

// the first row is for edge rise, the second row is for edge fall
// the i-th function in each row is for the i-th gpio pin
static GpioIrq irqs[2][32];

void add_irq(uint gpio, bool is_fall, GpioIrq callback) {
    irqs[is_fall][gpio] = callback;
    gpio_set_irq_enabled(gpio, is_fall ? GPIO_IRQ_EDGE_FALL : GPIO_IRQ_EDGE_RISE, true);
}

void register_irqs() {
    gpio_set_irq_callback(&gpio_irq);
}

struct repeating_timer ultra_trig_up_timer;
struct repeating_timer ultra_trig_down_timer;
struct repeating_timer motor_interrupt_timer;

void init() {
    stdio_init_all();
    cyw43_arch_init();

    register_irqs();

    enable();
}

void enable() {
    if (bobot_enabled)
        return;

    bobot_enabled = true;

    led.on();
}

void disable() {
    if (!bobot_enabled)
        return;

    bobot_enabled = false;

    led.off();
}

void toggle() {
    if (bobot_enabled) {
        disable();
    } else {
        enable();
    }
}

void gpio_irq(uint gpio, uint32_t event_mask) {
    bool is_fall = event_mask & GPIO_IRQ_EDGE_FALL;

    GpioIrq irq = irqs[is_fall][gpio];

    if (irq)
        irq();
}

} // namespace Bobot
