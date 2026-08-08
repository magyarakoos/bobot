#include <math.h>

#include "h_bridge.h"
#include "utils.h"

HBridge::HBridge(uint la_pin, uint lb_pin, uint ra_pin, uint rb_pin, uint freq)
    : la_pwm(la_pin, freq), lb_pwm(lb_pin, freq), ra_pwm(ra_pin, freq), rb_pwm(rb_pin, freq) {
    drive(0, 0);
}

void HBridge::drive(int l, int r) {
    l = clamp(l, -0xffff, 0xffff);
    r = clamp(r, -0xffff, 0xffff);

    if (abs(l) < 20) {
        l = 0;
    }

    if (abs(r) < 20) {
        r = 0;
    }

    if (l > 0) {
        la_pwm.duty_u16(0xffff);
        lb_pwm.duty_u16(0xffff - l);
    } else {
        la_pwm.duty_u16(0xffff + l);
        lb_pwm.duty_u16(0xffff);
    }

    if (r > 0) {
        ra_pwm.duty_u16(0xffff);
        rb_pwm.duty_u16(0xffff - r);
    } else {
        ra_pwm.duty_u16(0xffff + r);
        rb_pwm.duty_u16(0xffff);
    }
}
