#pragma once

#include "pico/stdlib.h"

// provides a basic abstraction for GPIO
// if unspecified, GPIO direction is OUT
//
// by default, the Pin is enabled
class Pin {
    bool out;
    bool _value;
    bool pull_up;

public:
    volatile bool inited;

    uint pin;

    Pin(uint _pin, bool _out, bool _pull_up);
    Pin(uint _pin, bool _out);
    Pin(uint _pin);

    void init();
    void deinit();

    // if this is an input pin, reads it and returns it
    // if this is an output pin, returns the last set value (or 0 if unset)
    bool get();

    // if this is an input pin, this does nothing
    // otherwise set the pin value
    void set(bool __value);

    // toggles the pin value
    // if the GPIO direction is IN, it doesn't do anything
    void toggle();
};
