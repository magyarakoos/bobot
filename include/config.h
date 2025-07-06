#pragma once

#include "pico/stdlib.h"

const uint BUZZER_PIN = 2;

const uint BUTTON_PIN = 0;
const uint BUTTON_DEBOUNCE_INTERVAL_MS = 500;

const uint ULTRA_TRIG_PIN = 20;
const uint ULTRA_ECHO_PIN = 19;

const uint HB_L1_PIN = 13;
const uint HB_L2_PIN = 12;
const uint HB_R1_PIN = 11;
const uint HB_R2_PIN = 10;
const uint HB_EEP_PIN = 15;
const uint HB_ULT_PIN = 14;
const uint HB_PWM_FREQ = 2000;

const uint SERVO_PIN = 16;
const float SERVO_MIN = 0.032f;
const float SERVO_MID = 0.075f;
const float SERVO_MAX = 0.130f;

const uint RGB_SENSOR_SDA_PIN = 26;
const uint RGB_SENSOR_SCL_PIN = 27;
const uint RGB_SENSOR_CHAN = 1;
const uint RGB_SENSOR_LED_PIN = 17;
const uint RGB_SENSOR_INTEGRATION_TIME = 1;
const uint RGB_SENSOR_GAIN = 1;

const uint PROXY_PIN = 3;

const uint ENC_LA = 6;
const uint ENC_LB = 7;
const uint ENC_RA = 8;
const uint ENC_RB = 9;
const uint ENC_BUFFER_SIZE = 256;
