#pragma once

#include "buzzer.h"
#include "onboard_led.h"
#include "pin.h"
#include "rgb_led.h"
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
    SpeedControl sc;
    Servo servo;
    TcpClient tcp;
    Pin track_left;
    Pin track_right;
    UltraSensor ultra;

    TcpError setup_tcp();
};
