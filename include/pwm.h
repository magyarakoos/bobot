#pragma once

#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "pin.h"

// abstraction for the hardware PWM's
// it's useful because we can configure it with frequency 
// and duty cycle, instead of wrap and level
//
// by default, the PWM is enabled
class PWM {
    uint pin;
    uint frequency;
    float duty_cycle;
    uint slice_num;
    uint channel_num;

    // based on the current frequency and duty cycle,
    // it calculates their corresponding wrap and level values
    // and uses them to set the PWM
    void calculate_values();

public:
    static const uint CLOCK_SPEED = 125e6; // CPU clock speed, 125 MHz
    // CPU clock divider, a higher value allowes much lower 
    // frequencies to be set, but the precision slightly decreases at
    // higher frequencies
    // For our use cases, this shouldn't be an issue
    // note: you can get the highest possible wrap value with CLOCK_SPEED / CLKDIV
    static const uint CLKDIV = 100;

    PWM(uint _pin);

    ~PWM();

    void enable();
    void disable();

    // sets the number of PWM cycles per second
    void freq(uint _frequency);

    // value between 0 and 1, representing the percentage 
    // of time the PWM signal is high within a single cycle
    void duty(float _duty_cycle);
};
  