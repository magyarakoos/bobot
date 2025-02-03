#pragma once 

#define uint unsigned int

const uint BUZZER_PIN = 4;

const uint BUTTON_PIN = 0;
const uint BUTTON_DEBOUNCE_INTERVAL_US = 5e5;

const uint ULTRA_TRIG_PIN = 20;
const uint ULTRA_ECHO_PIN = 19;

const uint HB_L1_PIN = 10;
const uint HB_L2_PIN = 11;
const uint HB_R1_PIN = 12;
const uint HB_R2_PIN = 13;
const uint HB_EEP_PIN = 14;
const uint HB_ULT_PIN = 15;
const uint HB_PWM_FREQ = 2000;

const uint SERVO_PIN = 16;
const float SERVO_MIN = 0.032f;
const float SERVO_MID = 0.075f;
const float SERVO_MAX = 0.130f;

const uint RGB_SENSOR_SDA_PIN = 26;
const uint RGB_SENSOR_SCL_PIN = 27;
const uint RGB_SENSOR_CHAN = 1;
const uint RGB_SENSOR_LED_PIN = 17;
const uint RGB_SENSOR_INTEGRATION_TIME = 40;
const uint RGB_SENSOR_GAIN = 3;

const uint ENC_LA = 6;
const uint ENC_LB = 7;
const uint ENC_RA = 8;
const uint ENC_RB = 9;
