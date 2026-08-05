#pragma once

#include <pico/types.h>

namespace config {

constexpr uint BUTTON_PIN = 0;

constexpr uint BUZZER_PIN = 14;

constexpr uint RGB_LED_R_PIN = 18;
constexpr uint RGB_LED_G_PIN = 19;
constexpr uint RGB_LED_B_PIN = 20;

constexpr uint HB_LA_PIN = 12;
constexpr uint HB_LB_PIN = 13;
constexpr uint HB_RA_PIN = 10;
constexpr uint HB_RB_PIN = 11;

constexpr uint ENC_LA_PIN = 6;
constexpr uint ENC_LB_PIN = 7;
constexpr uint ENC_RA_PIN = 8;
constexpr uint ENC_RB_PIN = 9;

constexpr float SC_PID_KP = 400.0;
constexpr float SC_PID_KI = 3200.0;
constexpr float SC_PID_KD = 0;
constexpr float SC_PID_INT_MIN = -45000.0;
constexpr float SC_PID_INT_MAX = 45000.0;

constexpr uint RGB_SENSOR_SDA_PIN = 16;
constexpr uint RGB_SENSOR_SCL_PIN = 17;
constexpr uint RGB_SENSOR_CHAN = 1;
constexpr uint RGB_SENSOR_LED_PIN = 28;
constexpr uint RGB_SENSOR_INTEGRATION_TIME = 40;
constexpr uint RGB_SENSOR_GAIN = 3;

constexpr uint SERVO_PIN = 22;
constexpr uint SERVO_MIN = 2100;
constexpr uint SERVO_MID = 4800;
constexpr uint SERVO_MAX = 8300;

constexpr uint ULTRA_TRIG_PIN = 27;
constexpr uint ULTRA_ECHO_PIN = 26;

} // namespace config
