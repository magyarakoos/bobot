#include "buzzer.h"

Buzzer::Buzzer(uint gpio, uint32_t freq_hz) : pwm_(gpio, freq_hz) {}

void Buzzer::freq(int freq_hz) {
    if (freq_hz > 0) {
        pwm_.freq(uint32_t(freq_hz));
        pwm_.duty_u16(0xffff / 2);
    } else {
        pwm_.duty_u16(0);
    }
}
