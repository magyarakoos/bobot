#include "servo.h"
#include "utils.h"

#define M_PI 3.1415926535

Servo::Servo(uint pin, float _min, float _mid, float _max) : pwm(pin), min(_min), mid(_mid), max(_max), inited(false) {}

void Servo::init() {
    if (inited)
        return;

    inited = true;

    pwm.init();
    pwm.freq(50);

    deg(0);
}

void Servo::deinit() {
    if (!inited)
        return;

    inited = false;

    pwm.deinit();
}

void Servo::duty(float d) {
    if (!inited)
        return;

    d = 1 - clamp(d, 0.0f, 1.0f);

    float duty;
    if (d <= 0.5)
        duty = min + ((mid - min) * d * 2);
    else
        duty = mid + ((max - mid) * (d * 2 - 1));

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
