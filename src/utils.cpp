#include "utils.h"

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
T max(const T& x, const T& y) {
    if (x < y) {
        return y;
    } else {
        return x;
    }
}
