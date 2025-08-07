#include "h_bridge.h"
#include "utils.h"

HBridge::HBridge(uint _l1, uint _l2, uint _r1, uint _r2, uint _eep, uint _ult, uint _pwm_freq)
    : l1(_l1),
      l2(_l2),
      r1(_r1),
      r2(_r2),
      pwm_freq(_pwm_freq),
      last_l(0),
      last_r(0),
      inited(false),
      l_speed(0),
      r_speed(0),
      eep(_eep),
      ult(_ult, GPIO_IN, true) {}

void HBridge::init() {
    if (inited && eep.get())
        return;

    inited = true;

    l1.init();
    l2.init();
    r1.init();
    r2.init();
    eep.init();
    ult.init();

    l1.freq(pwm_freq);
    l2.freq(pwm_freq);
    r1.freq(pwm_freq);
    r2.freq(pwm_freq);

    drive(last_l, last_r);

    eep.set(1);
}

void HBridge::deinit() {
    if (!inited)
        return;

    inited = false;

    drive(0, 0);

    last_l = l_speed;
    last_r = r_speed;

    l1.deinit();
    l2.deinit();
    r1.deinit();
    r2.deinit();
    eep.deinit();
    ult.deinit();
}

void HBridge::drive(float l, float r) {
    if (!inited)
        return;

    l *= 0.01f;
    r *= 0.01f;

    l = clamp(l, -1.0f, 1.0f);
    r = clamp(r, -1.0f, 1.0f);

    l1.duty(abs(l) * (l > 0));
    l2.duty(abs(l) * (l < 0));

    r1.duty(abs(r) * (r < 0));
    r2.duty(abs(r) * (r > 0));

    l_speed = l;
    r_speed = r;
}
