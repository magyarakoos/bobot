#pragma once

#include "pico/stdlib.h"

// provides a basic abstraction for GPIO
// if unspecified, GPIO direction is OUT
//
// by default, the Pin is enabled
class Pin {
    bool out;
    bool _value;

public:
    uint pin;

    Pin(uint _pin, bool _out, bool _pull_up);
    Pin(uint _pin, bool _out);
    Pin(uint _pin);

    ~Pin();

    void enable();
    void disable();

    // the behavior is dependant on the GPIO direction:
    // if it's  IN: returns the value of the pin
    // if it's OUT: sets the value to the pin, returns 0
    bool value(bool __value = 0);

    // toggles the pin value
    // if the GPIO direction is IN, it doesn't do anything
    void toggle();
};
