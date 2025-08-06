#pragma once

#include <stdint.h>

#ifdef __cplusplus

template <typename T>
T clamp(const T& value, const T& min, const T& max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

template <typename T>
T abs(const T& x) {
    if (x < 0) {
        return -x;
    } else {
        return x;
    }
}

template <typename T>
T min(const T& x, const T& y) {
    if (y < x) {
        return y;
    } else {
        return x;
    }
}

template <typename T>
T max(const T& x, const T& y) {
    if (y > x) {
        return y;
    } else {
        return x;
    }
}

template <typename T>
T sign(const T& x) {
    if (x > 0)
        return 1;
    if (x < 0)
        return -1;
    return 0;
}

#endif

typedef struct hsv_color {
    float h, s, v;
} hsv_color;

hsv_color rgb_to_hsv(uint16_t r, uint16_t g, uint16_t b);

#ifdef __cplusplus

#include <array>

std::array<float, 3> hsv_to_rgb(hsv_color c);

#endif

// void util_rgb_rel(rgb_sensor_color_raw* raw);

#define CLAMP(v, min, max) ((v) > (max) ? (max) : ((v) < (min) ? (min) : (v)))

#define _UTIL_QUOTE(str) #str
#define _UTIL_EXPAND_AND_QUOTE(str) _UTIL_QUOTE(str)
