#include "rgb_led.h"
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
}

void RGBLed::deinit() {
    if (!inited)
        return;

    inited = true;

    r.deinit();
    g.deinit();
    b.deinit();
}

void RGBLed::set_color(float _r, float _g, float _b) {
    if (!inited)
        return;

    _r = clamp(_r, 0.0f, 1.0f);
    _g = clamp(_r, 0.0f, 1.0f);
    _b = clamp(_r, 0.0f, 1.0f);

    r.duty(1.0f - _r);
    g.duty(1.0f - _g);
    b.duty(1.0f - _b);
}
