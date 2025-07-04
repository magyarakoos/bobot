#include "rgb_led.h"
#include "utils.h"

RGBLed::RGBLed(uint _r, uint _g, uint _b, uint freq) : r(_r), g(_g), b(_b) {
    r.freq(freq);
    g.freq(freq);
    b.freq(freq);
}

void RGBLed::set_color(float _r, float _g, float _b) {
    _r = clamp(_r, 0.0f, 1.0f);
    _g = clamp(_r, 0.0f, 1.0f);
    _b = clamp(_r, 0.0f, 1.0f);

    r.duty(1.0f - _r);
    g.duty(1.0f - _g);
    b.duty(1.0f - _b);
}
