#include "bobot.h"

#include "config.h"

Bobot::Bobot()
    : button(config::BUTTON_PIN, Pin::Direction::In, Pin::Pull::Up),
      buzzer(config::BUZZER_PIN),
      rgb_led(config::RGB_LED_R_PIN, config::RGB_LED_G_PIN, config::RGB_LED_B_PIN),
      track_left(config::TRACK_LEFT_PIN, Pin::Direction::In),
      track_right(config::TRACK_RIGHT_PIN, Pin::Direction::In),
      ultra(config::ULTRA_TRIG_PIN, config::ULTRA_ECHO_PIN),
      servo(config::SERVO_PIN, config::SERVO_MIN, config::SERVO_MID, config::SERVO_MAX),
      sc(config::HB_LA_PIN,
         config::HB_LB_PIN,
         config::HB_RA_PIN,
         config::HB_RB_PIN,
         config::ENC_LA_PIN,
         config::ENC_LB_PIN,
         config::ENC_RA_PIN,
         config::ENC_RB_PIN,
         config::SC_PID_KP,
         config::SC_PID_KI,
         config::SC_PID_KD,
         config::SC_PID_INT_MIN,
         config::SC_PID_INT_MAX) {}
