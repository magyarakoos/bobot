#include "utils.h"
#include <array>
#include <cmath>

hsv_color rgb_to_hsv(uint16_t rr, uint16_t gg, uint16_t bb) {
    float r = (float) rr / 0xffff;
    float g = (float) gg / 0xffff;
    float b = (float) bb / 0xffff;

    float mx = max(max(r, g), b);
    float mn = min(min(r, g), b);
    float delta = mx - mn;

    hsv_color hsv = (hsv_color) {
        .h = 0,
        .s = mx == 0 ? 0 : (mx - mn) / mx,
        .v = mx,
    };

    if (delta == 0)
        return hsv;

    if (r == mx) {
        hsv.h = (g - b) / delta;
    } else {
        if (g == mx)
            hsv.h = 2 + (b - r) / delta;
        else
            hsv.h = 4 + (r - g) / delta;
    }

    hsv.h *= 60;
    if (hsv.h < 0)
        hsv.h += 360;

    return hsv;
}

#ifdef __cplusplus

std::array<float, 3> hsv_to_rgb(hsv_color c) {
    float r, g, b;

    float h = c.h / 360.0;
    float s = c.s;
    float v = c.v;

    int i = floor(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    return { r, g, b };
}
#endif
