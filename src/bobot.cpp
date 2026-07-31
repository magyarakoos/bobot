#include "bobot.h"

#include "config.h"
#include "net_config.h"
#include "tcp.h"

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

void Bobot::tcp_packet_handler(const uint8_t* payload, uint32_t length) {
    printf("Received packet (%u bytes): %.*s\n", length, (int) length, payload);
}

TcpError Bobot::setup_tcp() {
    auto err = tcp.init_wifi(config::WIFI_SSID, config::WIFI_PASSWORD, config::WIFI_TIMEOUT_MS);
    if (err != TcpError::OK) {
        return err;
    }

    tcp.set_packet_callback([this](const uint8_t* payload, uint32_t length) { tcp_packet_handler(payload, length); });

    return tcp.connect(config::SERVER_IP, config::SERVER_PORT);
}
