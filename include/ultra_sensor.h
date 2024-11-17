#pragma once

#include "pico/stdlib.h"
#include "pin.h"
#include <optional>

class UltraSensor {
    Pin trig;
    Pin echo;
    uint buffer_size;

    // conversion constant for the speed of sound
    static const float METERS_PER_US = 0.0001715f;
    // minimum range of the sensor
    static const float MIN_DIST = 0.02f;
    // maximum range of the sensor
    static const float MAX_DIST = 4.0f;
public:
    UltraSensor(uint _trig, uint _echo, uint _buffer_size = 5);

    void enable();
    void disable();

    // returns the distance measured by the sensor, or
    // std::nullopt if it was outside it's rated range
    std::optional<float> measure_sync();

    // it has the same functionality as measure_sync,
    // but it uses a buffer to store previous measurements
    // for more accurate results
    std::optional<float> measure_precise_sync();
};