#include "h_bridge.h"
#include "pico/stdlib.h"

#include <algorithm>

HBridge::HBridge(uint _l1, uint _l2, uint _r1, uint _r2,
                 uint _eep, uint _ult, uint pwm_freq)
    : l1(_l1), l2(_l2), r1(_r1), r2(_r2), eep(_eep),
      ult(_ult, GPIO_IN) 
    {
    enable();

    l1.freq(pwm_freq);
    l2.freq(pwm_freq);
    r1.freq(pwm_freq);
    r2.freq(pwm_freq);
}

void HBridge::enable() {
    l1.enable();
    l2.enable();
    r1.enable();
    r2.enable();
    eep.enable();
    ult.enable();

    eep.value(1);
}

void HBridge::disable() {
    eep.value(0);

    l1.disable();
    l2.disable();
    r1.disable();
    r2.disable();
    eep.disable();
    ult.disable();
}

void HBridge::drive(float l, float r) {
    l = std::clamp(l, -1.0f, 1.0f);
    r = std::clamp(r, -1.0f, 1.0f);

    l1.duty(abs(l) * (l > 0));
    l2.duty(abs(l) * (l < 0));

    r1.duty(abs(r) * (r < 0));
    r2.duty(abs(r) * (r > 0));
}
