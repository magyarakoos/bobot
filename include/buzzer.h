#pragma once

#include "pwm.h"

class Buzzer {
public:
    explicit Buzzer(uint gpio, uint32_t freq_hz = 2000);

    void freq(int freq_hz);

private:
    Pwm pwm_;
};
