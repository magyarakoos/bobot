#include "rgb_led.h"
#include <cstdio>
#include "utils.h"

RGBLed::RGBLed(uint _r, uint _g, uint _b, uint _freq) : r(_r), g(_g), b(_b), freq(_freq), inited(false) {}

void RGBLed::init() {
    if (inited)
        return;

    inited = true;

    r.init();
    g.init();
    b.init();

    r.freq(freq);
    g.freq(freq);
    b.freq(freq);

    r.duty(0);
    g.duty(0);
    b.duty(0);
}

void RGBLed::deinit() {
    if (!inited)
        return;

    inited = false;

    r.deinit();
    g.deinit();
    b.deinit();
}

void RGBLed::set_color(float _r, float _g, float _b) {
    if (!inited)
        return;

    _r = 1.0f - clamp(_r, 0.0f, 1.0f);
    _g = 1.0f - clamp(_g, 0.0f, 1.0f);
    _b = 1.0f - clamp(_b, 0.0f, 1.0f);

    r.duty(_r);
    g.duty(_g);
    b.duty(_b);
}
