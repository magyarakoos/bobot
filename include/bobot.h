#pragma once

#include "buzzer.h"
#include "pin.h"
#include "rgb_led.h"
#include "servo.h"
#include "speed_control.h"
#include "ultra_sensor.h"

class Bobot {
public:
    Bobot();

    Pin button;
    Buzzer buzzer;
    RGBLed rgb_led;
    Pin track_left;
    Pin track_right;
    UltraSensor ultra;
    Servo servo;
    SpeedControl sc;
};
