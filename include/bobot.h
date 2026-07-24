#pragma once

#include "buzzer.h"
#include "pin.h"
#include "rgb_led.h"
#include "rgb_sensor.h"
#include "servo.h"
#include "speed_control.h"
#include "ultra_sensor.h"

class Bobot {
public:
    Bobot();

    Pin button;
    Buzzer buzzer;
    RGBLed rgb_led;
    RgbSensor rgb_sensor;
    UltraSensor ultra;
    Servo servo;
    SpeedControl sc;
};
