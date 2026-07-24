#include "servo.h"
#include "utils.h"

Servo::Servo(uint pin, uint _min, uint _mid, uint _max) : pwm(pin, 50), min(_min), mid(_mid), max(_max) {
    deg(0);
}

void Servo::duty(uint16_t d) {
    d = 0xffff - d;

    if (d < 0xffff / 2) {
        d = min + (mid - min) * d * 2 / 0xffff;
    } else {
        d = mid + (max - mid) * (d - 0xffff / 2) * 2 / 0xffff;
    }

    pwm.duty_u16(d);
}

void Servo::deg(int d) {
    d = clamp(d, -90, 90);
    duty((d + 90) * 0xffff / 180);
}
