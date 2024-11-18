#include "ultra_sensor.h"

UltraSensor::UltraSensor(uint _trig, uint _echo,
                         uint _buffer_size) 
    : trig(_trig), echo(_echo), buffer_size(_buffer_size)
    {
        
}
