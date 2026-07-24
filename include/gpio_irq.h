#pragma once
#include <array>
#include <functional>
#include "hardware/gpio.h"

class GpioIrq {
public:
    using Callback = std::function<void(uint gpio, uint32_t events)>;

    static void attach(uint gpio, uint32_t events, Callback cb);
    static void detach(uint gpio);

private:
    static std::array<Callback, NUM_BANK0_GPIOS> _callbacks;
    static bool _initialized;

    static void sdk_router(uint gpio, uint32_t events);
};
