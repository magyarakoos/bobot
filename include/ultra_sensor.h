#pragma once

#include "pico/stdlib.h"
#include "pin.h"
#include <optional>

// Ultrasound sensor controller
class UltraSensor {
    bool enabled = true;

    // conversion constant for the speed of sound
    static constexpr float METERS_PER_US = 0.0001715f;
    // minimum range of the sensor
    static constexpr float MIN_DIST = 0.02f;
    // maximum range of the sensor
    static constexpr float MAX_DIST = 4.0f;

public:
    Pin trig; 
    Pin echo;

    volatile uint64_t rise;
    volatile uint64_t fall;
    volatile uint64_t last_pulse;

    UltraSensor(uint _trig, uint _echo);

    void enable();
    void disable();

    float distance();
};