#include "ultra_sensor.h"

UltraSensor::UltraSensor(uint _trig, uint _echo,
                         uint _buffer_size) 
    : trig(_trig), echo(_echo), buffer_size(_buffer_size)
    {
        
}

std::optional<float> UltraSensor::measure_sync() {
    trig.value(1);
    sleep_us(15);
    trig.value(0);

    
}

std::optional<float> UltraSensor::measure_precise_sync() {
    return std::nullopt;
}
