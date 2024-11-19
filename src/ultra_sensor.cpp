#include "ultra_sensor.h"

UltraSensor::UltraSensor(uint _trig, uint _echo) 
    : trig(_trig), echo(_echo, GPIO_IN) 
    {
         
}

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

void UltraSensor::send_trigger() {
    trig.value(1);
    sleep_us(15);
    trig.value(0);

    last_trigger_us = time_us_64();
}

std::optional<float> UltraSensor::measure() {
    float distance = last_time_elapsed_us * METERS_PER_US;

    if (distance < MIN_DIST || distance > MAX_DIST || !enabled) {
        return std::nullopt;
    }

    return distance;
}
