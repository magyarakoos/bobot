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

constexpr float SC_PID_KP = 8000;
constexpr float SC_PID_KI = 0000;
constexpr float SC_PID_KD = 0;
constexpr float SC_PID_INT_MIN = -45000;
constexpr float SC_PID_INT_MAX = 45000;

constexpr uint TRACK_LEFT_PIN = 28;
constexpr uint TRACK_RIGHT_PIN = 16;

constexpr uint SERVO_PIN = 22;
constexpr uint SERVO_MIN = 2100;
constexpr uint SERVO_MID = 4800;
constexpr uint SERVO_MAX = 8300;

constexpr uint ULTRA_TRIG_PIN = 27;
constexpr uint ULTRA_ECHO_PIN = 26;

} // namespace config
