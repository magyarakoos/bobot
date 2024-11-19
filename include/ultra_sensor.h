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

    uint64_t last_trigger_us = 0;
    uint64_t last_time_elapsed_us = 0;

    UltraSensor(uint _trig, uint _echo);

    void enable();
    void disable();

    // sends a value to the trigger pin, this starts
    // the callback loop, this must be called after
    // the irq callback got registered
    void send_trigger();

    // returns the distance measured by the sensor (in meters)
    // or std::nullopt if it was outside it's rated range
    // or the sensor is currently disabled
    std::optional<float> measure();
};