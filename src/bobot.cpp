#include "bobot.h"
#include <hardware/gpio.h>
#include <cstdio>
#include "config.h"
#include "encoder.h"

namespace Bobot {

PWM buzzer(BUZZER_PIN);
OnboardLed led;
HBridge hb(HB_L1_PIN, HB_L2_PIN, HB_R1_PIN, HB_R2_PIN, HB_EEP_PIN, HB_ULT_PIN, HB_PWM_FREQ);
Pin button(BUTTON_PIN, GPIO_IN, true);
UltraSensor ultra(ULTRA_TRIG_PIN, ULTRA_ECHO_PIN);
RgbSensor rgb_sensor(RGB_SENSOR_SDA_PIN,
                     RGB_SENSOR_SCL_PIN,
                     RGB_SENSOR_CHAN,
                     RGB_SENSOR_LED_PIN,
                     RGB_SENSOR_INTEGRATION_TIME,
                     RGB_SENSOR_GAIN);
Servo servo(SERVO_PIN, SERVO_MIN, SERVO_MID, SERVO_MAX);
Pin proxy(PROXY_PIN, GPIO_IN);
Encoder enc_left(ENC_LA, ENC_LB);
Encoder enc_right(ENC_RA, ENC_RB);

// the first row is for edge rise, the second row is for edge fall
// the i-th function in each row is for the i-th gpio pin
static GpioIrq irqs[2][32];

void register_irqs() {
    gpio_set_irq_callback(&gpio_irq);

    // Ultra sensor
    irqs[0][ultra.echo.pin] = []() { ultra.rise = time_us_64(); };
    irqs[1][ultra.echo.pin] = []() { ultra.fall = time_us_64(); };

    // Pause button
    irqs[1][button.pin] = []() {
        uint64_t now = time_us_64();
        if (now - last_pause_us >= BUTTON_DEBOUNCE_DELAY_MS * 1000) {
            last_pause_us = now;
            toggle();
        }
    };

    // Encoder
    irqs[0][enc_left.A.pin] = []() { enc_left.callback_a_rise(); };
    irqs[0][enc_right.A.pin] = []() { enc_right.callback_a_rise(); };
}

void enable_irqs(bool enabled) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 32; j++) {
            if (irqs[i][j]) {
                uint32_t mask = i ? GPIO_IRQ_EDGE_FALL : GPIO_IRQ_EDGE_RISE;
                gpio_set_irq_enabled(j, mask, enabled);
            }
        }
    }
}

struct repeating_timer ultra_trig_up_timer;
struct repeating_timer ultra_trig_down_timer;

void register_timers() {

    { // Ultra sensor
        add_repeating_timer_ms(
            -60,
            [](__unused repeating_timer* t) {
                ultra.trig.value(1);
                ultra.last_pulse = time_us_64();
                return true;
            },
            NULL, &ultra_trig_up_timer);

        sleep_us(15);

        add_repeating_timer_ms(
            -60,
            [](__unused repeating_timer* t) {
                ultra.trig.value(0);
                return true;
            },
            NULL, &ultra_trig_down_timer);
    }
}

void init() {
    stdio_init_all();

    register_irqs();
    register_timers();

    enable();

    servo.deg(0);
    rgb_sensor.led.value(0);
}

void enable() {
    if (bobot_enabled)
        return;

    bobot_enabled = true;

    enable_irqs(true);

    hb.enable();
    buzzer.enable();
    proxy.enable();
    ultra.enable();
    servo.enable();
    led.on();
}

void disable() {
    if (!bobot_enabled)
        return;

    bobot_enabled = false;

    enable_irqs(false);

    hb.disable();
    buzzer.disable();
    proxy.disable();
    ultra.disable();
    servo.disable();
    led.off();
}

void toggle() {
    (bobot_enabled ? disable : enable)();
}

void gpio_irq(uint gpio, uint32_t event_mask) {
    bool is_fall = event_mask & GPIO_IRQ_EDGE_FALL;

    GpioIrq irq = irqs[is_fall][gpio];

    if (irq)
        irq();
}

void print(const char* fmt, ...) {
    enable_irqs(false);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    enable_irqs(true);
}

} // namespace Bobot
