#include <cstdint>

template <typename T>
T clamp(T x, T min, T max) {
    if (x < min) {
        return min;
    }
    if (x > max) {
        return max;
    }
    return x;
}

class Pid {
public:
    float kp;
    float ki;
    float kd;

    float int_min;
    float int_max;

    float sp;

    Pid(float _kp, float _ki, float _kd, float _int_min, float _int_max, float _sp);

    float step(float pv, float dt);

private:
    float _int;
    float _last_e;
};

struct rgb_color {
    uint8_t r, g, b;
};

rgb_color hsv_to_rgb(float h, float s, float v);
