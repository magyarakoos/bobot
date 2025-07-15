#pragma once

#include "config.h"
#include "pin.h"
#include "ring_buffer.h"

// Ultrasound sensor controller
class UltraSensor {

    // conversion constant for the speed of sound
    static constexpr float METERS_PER_US = 0.000175f;
    // minimum range of the sensor
    static constexpr float MIN_DIST = 0.02f;
    // maximum range of the sensor
    static constexpr float MAX_DIST = 4.0f;

    volatile uint64_t last_dist;

    volatile bool inited;

    struct Datapoint {
        uint64_t t;
        float dist;
    };

    RingBuffer<Datapoint, ULTRA_BUFFER_SIZE> buffer;

    float rolling_dist_sum;

public:
    Pin trig;
    Pin echo;

    volatile uint64_t rise;
    volatile uint64_t fall;
    volatile uint64_t last_pulse;

    UltraSensor(uint _trig, uint _echo);

    void init();
    void deinit();

    // returns the currently measured distance
    //
    // NOTE: it is preferred to use `distance_smooth` instead for better accuracy
    float distance();

    // returns the rolling average of the last few valid measurements
    float distance_smooth();

    // call this on echo edge rise
    void callback_echo_rise();
    // call this on echo edge fall
    void callback_echo_fall();

    // call this every 60ms
    void timer_callback_trig_up();
    // call this every 60ms as well, but with a 15us delayed start
    void timer_callback_trig_down();
};
