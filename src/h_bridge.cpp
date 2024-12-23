#include "h_bridge.h"
#include "pico/stdlib.h"

#include <cmath>
#include <algorithm>

HBridge::HBridge(uint _l1, uint _l2, uint _r1, uint _r2, uint _eep, uint _ult, uint pwm_freq)
    : l1(_l1), l2(_l2), r1(_r1), r2(_r2), eep(_eep), ult(_ult, GPIO_IN) {
    
    eep.value(1);

    l1.freq(pwm_freq);
    l2.freq(pwm_freq);
    r1.freq(pwm_freq);
    r2.freq(pwm_freq);
}

void HBridge::drive(float l, float r) {
    l_speed = std::clamp(l, -1.0f, 1.0f);
    r_speed = std::clamp(r, -1.0f, 1.0f);

    l1.duty(fabsf(l_speed) * (l_speed > 0));
    l2.duty(fabsf(l_speed) * (l_speed < 0));

    r1.duty(fabsf(r_speed) * (r_speed < 0));
    r2.duty(fabsf(r_speed) * (r_speed > 0));
}
