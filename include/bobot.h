#pragma once

#include "buzzer.h"
#include "onboard_led.h"
#include "pin.h"
#include "rgb_led.h"
#include "rgb_sensor.h"
#include "servo.h"
#include "speed_control.h"
#include "tcp.h"
#include "ultra_sensor.h"

class Bobot {
    void tcp_packet_handler(const uint8_t* payload, uint32_t length);

public:
    Bobot();

    Pin button;
    Buzzer buzzer;
    OnboardLed led;
    RGBLed rgb_led;
    RgbSensor rgb_sensor;
    Servo servo;
    SpeedControl sc;
    TcpClient tcp;
    UltraSensor ultra;

    TcpError setup_tcp();
};
