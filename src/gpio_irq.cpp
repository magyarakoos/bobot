#include "gpio_irq.h"

std::array<GpioIrq::Callback, NUM_BANK0_GPIOS> GpioIrq::_callbacks;
bool GpioIrq::_initialized = false;

void GpioIrq::attach(uint gpio, uint32_t events, Callback cb) {
    _callbacks[gpio] = cb;

    if (!_initialized) {
        gpio_set_irq_enabled_with_callback(gpio, events, true, &GpioIrq::sdk_router);
        _initialized = true;
    } else {
        gpio_set_irq_enabled(gpio, events, true);
    }
}

void GpioIrq::detach(uint gpio) {
    gpio_set_irq_enabled(gpio, 0xFF, false);
    _callbacks[gpio] = nullptr;
}

void __not_in_flash_func(GpioIrq::sdk_router)(uint gpio, uint32_t events) {
    if (gpio < NUM_BANK0_GPIOS && _callbacks[gpio]) {
        _callbacks[gpio](gpio, events);
    }
}
