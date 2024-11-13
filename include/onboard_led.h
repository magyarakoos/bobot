#pragma once

#include "pico/stdlib.h"

// the onboard led on the Pico W isn't controlled
// by simple GPIO, it's done through the wifi driver
// https://github.com/raspberrypi/pico-examples/blob/7e77a0c381863be0c49086567e7f1934d78ac591/blink/blink.c#L40C5-L40C56
class OnboardLed {
    uint led_pin;
    bool value;
public:
    OnboardLed();
    void on();
    void off();
    void toggle();
};