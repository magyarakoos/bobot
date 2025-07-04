#include "h_bridge.h"
#include "pico/stdlib.h"
#include "utils.h"

HBridge::HBridge(uint _l1, uint _l2, uint _r1, uint _r2, uint _eep, uint _ult, uint pwm_freq)
    : l1(_l1), l2(_l2), r1(_r1), r2(_r2), eep(_eep), last_l_speed(0), last_r_speed(0), ult(_ult, GPIO_IN, true) {

    l1.freq(pwm_freq);
    l2.freq(pwm_freq);
    r1.freq(pwm_freq);
    r2.freq(pwm_freq);

    enable();
}

void HBridge::enable() {
    eep.value(1);
    drive(last_l_speed, last_r_speed);
}

void HBridge::disable() {
    last_l_speed = l_speed;
    last_r_speed = r_speed;
    drive(0, 0);
    eep.value(0);
}

void HBridge::drive_raw(float l, float r) {

    l1.duty(abs(l) * (l > 0));
    l2.duty(abs(l) * (l < 0));

    r1.duty(abs(r) * (r > 0));
    r2.duty(abs(r) * (r < 0));
}

void HBridge::drive(float l, float r) {

    l = clamp(l, -1.0f, 1.0f);
    r = clamp(r, -1.0f, 1.0f);

    int step_count = max(abs(l - l_speed), abs(r - r_speed)) / STEP_SIZE;
    for (int i = 0; i <= step_count; i++) {
        float t = (float) i / step_count;

        drive_raw(l * t + l_speed * (1.0f - t), r * t + r_speed * (1.0f - t));

        for (volatile int j = 0; j < DELAY_CYCLES; j++)
            ;
    }

    l_speed = l;
    r_speed = r;
}
