#include "ultra_sensor.h"

UltraSensor::UltraSensor(uint _trig, uint _echo) 
    : trig(_trig), echo(_echo, GPIO_IN), distance(-1)
    {}

void UltraSensor::enable() {
    if (enabled) return;
    enabled =  true;

    trig.enable();
    echo.enable();
}

void UltraSensor::disable() {
    if (!enabled) return;
    enabled = false;

    trig.disable();
    echo.disable();
}

float UltraSensor::calculate_distance(uint64_t elapsed) {
    float distance = elapsed * METERS_PER_US;

    if (distance < MIN_DIST || distance > MAX_DIST || !enabled) {
        return -1;
    }

    return distance;
}
