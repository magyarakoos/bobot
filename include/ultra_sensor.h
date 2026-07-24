#pragma once
#include <pico/stdlib.h>
#include "pico/time.h"

class UltraSensor {
public:
    static constexpr float MIN_DIST = 2.0f;
    static constexpr float MAX_DIST = 400.0f;
    static constexpr float CM_PER_US = 0.01715f;
    static constexpr uint32_t TRIGGER_PERIOD_MS = 60;
    static constexpr uint32_t TRIGGER_PULSE_US = 15;
    static constexpr uint32_t TIME_WINDOW_US = 2e5;

    UltraSensor(uint trig_gpio, uint echo_gpio);

    float dist();

private:
    uint trig_;
    uint echo_;

    uint32_t rise_ = 0;

    volatile uint32_t pulse_width_us_ = 0;
    volatile uint32_t last_measurement_us_ = 0;

    repeating_timer_t trigger_timer_{};
};
