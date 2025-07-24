#include "ultra_sensor.h"

UltraSensor::UltraSensor(uint _trig, uint _echo)
    : last_dist(0),
      inited(false),
      rolling_dist_sum(0),
      trig(_trig),
      echo(_echo, GPIO_IN),
      rise(0),
      fall(0),
      last_pulse(0) {}

void UltraSensor::init() {
    if (inited)
        return;

    inited = true;

    trig.init();
    echo.init();
}

void UltraSensor::deinit() {
    if (!inited)
        return;

    inited = false;

    trig.deinit();
    echo.deinit();
}

float UltraSensor::distance() {
    if (!inited)
        return -1;

    float dist;
    if (rise > fall) {
        dist = last_dist;
    } else {
        dist = (fall - rise) * METERS_PER_US;
        last_dist = dist;
    }

    if (dist < MIN_DIST || dist > MAX_DIST)
        return -1;

    rolling_dist_sum += dist;
    buffer.push_back({ time_us_64(), dist });
    return dist;
}

float UltraSensor::distance_smooth() {
    distance();

    // remove old values
    uint64_t now = time_us_64();
    while (!buffer.empty() && buffer.front().t + ULTRA_BUFFER_TIME_WINDOW_US < now) {
        rolling_dist_sum -= buffer.front().dist;
        buffer.pop_front();
    }

    if (buffer.empty()) {
        return -1.0;
    } else {
        return rolling_dist_sum / buffer.size();
    }
}

void UltraSensor::callback_echo_rise() {
    if (!inited)
        return;

    rise = time_us_64();
}

void UltraSensor::callback_echo_fall() {
    if (!inited)
        return;

    fall = time_us_64();
}

void UltraSensor::timer_callback_trig_up() {
    if (!inited)
        return;

    trig.set(1);
    last_pulse = time_us_64();
}

void UltraSensor::timer_callback_trig_down() {
    if (!inited)
        return;

    trig.set(0);
}
