#include "ultra_sensor.h"

UltraSensor::UltraSensor(uint _trig, uint _echo) : trig(_trig), echo(_echo, GPIO_IN) {
    enabled = true;
    last_dist = 0;
    rise = 0;
    fall = 0;
    last_pulse = 0;
}

void UltraSensor::enable() {
    if (enabled)
        return;

    enabled = true;

    trig.enable();
    echo.enable();
}

void UltraSensor::disable() {
    if (!enabled)
        return;

    enabled = false;

    trig.disable();
    echo.disable();
}

float UltraSensor::distance() {

    float dist;
    if (rise > fall) {
        dist = last_dist;
    } else {
        dist = (fall - rise) * METERS_PER_US;
        last_dist = dist;
    }

    if (dist < MIN_DIST || dist > MAX_DIST)
        return -1;

    return dist;
}
