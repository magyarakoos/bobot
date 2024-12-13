#include "ultra_sensor.h"

UltraSensor::UltraSensor(uint _trig, uint _echo) : trig(_trig), echo(_echo, GPIO_IN) {}

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
    float distance = (fall - rise) * METERS_PER_US;

    if (distance < MIN_DIST || distance > MAX_DIST || !enabled)
        return -1;

    return distance;
}
