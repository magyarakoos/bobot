#include "servo.h"
#include <algorithm>
#include <cmath>

Servo::Servo(uint pin, uint16_t min, uint16_t mid, uint16_t max) : pwm(PWM(pin)) {
    this->min = min;
    this->mid = mid;
    this->max = max;

    pwm.freq(2000);
}

void Servo::duty(float d) {
    d = 1 - std::clamp(d, 0.0f, 1.0f);

    float duty;
    if (d <= 0.5)
        duty = min + (int) ((mid - min) * d * 2);
    else
        duty = mid + (int) ((max - mid) * (d * 2 - 1));

    pwm.duty(duty);
}

void Servo::deg(float d) {
    d = (d + 90) / 180;
    duty(d);
}

void Servo::rad(float r) {
    r = (r + 0.5 * M_PI) / M_PI;
    duty(r);
}
