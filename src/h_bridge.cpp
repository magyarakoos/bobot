#include "h_bridge.h"
#include "pico/stdlib.h"

#include <algorithm>
#include <cmath>

HBridge::HBridge(uint _l1, uint _l2, uint _l_pwm, uint _r1, uint _r2, uint _r_pwm, uint pwm_freq)
    : l1(_l1), l2(_l2), l_pwm(_l_pwm), r1(_r1), r2(_r2), r_pwm(_r_pwm), last_l_speed(0), last_r_speed(0) {

    l_pwm.freq(pwm_freq);
    r_pwm.freq(pwm_freq);

    enable();
}

void HBridge::enable() {
    drive(last_l_speed, last_r_speed);
}

void HBridge::disable() {
    last_l_speed = l_speed;
    last_r_speed = r_speed;
    drive(0, 0);
}

void HBridge::drive(float l, float r) {
    l_speed = std::clamp(l, -1.0f, 1.0f);
    r_speed = std::clamp(r, -1.0f, 1.0f);

    l1.value(l_speed < 0);
    l2.value(l_speed > 0);
    l_pwm.duty(abs(l_speed));

    r1.value(r_speed > 0);
    r2.value(r_speed < 0);
    r_pwm.duty(abs(r_speed));
}
