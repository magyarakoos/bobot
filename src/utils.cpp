#include "utils.h"

Pid::Pid(float _kp, float _ki, float _kd, float _int_min, float _int_max, float _sp)
    : kp(_kp), ki(_ki), kd(_kd), int_min(_int_min), int_max(_int_max), sp(_sp), _int(0.0), _last_e(0.0) {}

float Pid::step(float pv, float dt) {
    float e = sp - pv;

    _int = clamp(_int + ki * e * dt, int_min, int_max);

    float p = kp * e;
    float i = _int;
    float d = kd * (e - _last_e) / dt;

    last_p = p;
    last_i = i;
    last_d = d;

    _last_e = e;
    return p + i + d;
}

rgb_color hsv_to_rgb(float h, float s, float v) {
    int i = h / 60.0f;
    float f = (h / 60.0f) - (float) i;

    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    float r, g, b;
    switch (i % 6) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        case 5:
            r = v;
            g = p;
            b = q;
            break;
    }

    uint8_t r_byte = clamp(r, 0.0f, 1.0f) * 255 + 0.5f;
    uint8_t g_byte = clamp(g, 0.0f, 1.0f) * 255 + 0.5f;
    uint8_t b_byte = clamp(b, 0.0f, 1.0f) * 255 + 0.5f;

    return { r_byte, g_byte, b_byte };
}
