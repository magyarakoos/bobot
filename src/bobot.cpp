#include "bobot.h"
#include <pico/cyw43_arch.h>
#include <cstdio>
#include "config.h"
#include "encoder.h"
#include "motor.h"

namespace Bobot {

PWM buzzer(BUZZER_PIN);
OnboardLed led;
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
Motor motor(HB_L1_PIN,
            HB_L2_PIN,
            HB_R1_PIN,
            HB_R2_PIN,
            HB_EEP_PIN,
            HB_ULT_PIN,
            HB_PWM_FREQ,
            ENC_LA,
            ENC_LB,
            ENC_RA,
            ENC_RB,
            20,
            30,
            0,
            -70,
            70);
RGBLed rgb_led(RGB_LED_R_PIN, RGB_LED_G_PIN, RGB_LED_B_PIN, RGB_LED_PWM_FREQ);

// the first row is for edge rise, the second row is for edge fall
// the i-th function in each row is for the i-th gpio pin
static GpioIrq irqs[2][32];

void add_irq(uint gpio, bool is_fall, GpioIrq callback) {
    irqs[is_fall][gpio] = callback;
    gpio_set_irq_enabled(gpio, is_fall ? GPIO_IRQ_EDGE_FALL : GPIO_IRQ_EDGE_RISE, true);
}

void register_irqs() {
    gpio_set_irq_callback(&gpio_irq);

    // Ultra sensor
    add_irq(ultra.echo.pin, false, []() { ultra.callback_echo_rise(); });
    add_irq(ultra.echo.pin, true, []() { ultra.callback_echo_fall(); });

    // Pause button
    add_irq(button.pin, true, []() {
        if (!button.inited)
            return;

        uint64_t now = time_us_64();
        if (now - last_pause_us >= BUTTON_DEBOUNCE_DELAY_US) {
            last_pause_us = now;
            toggle();
        }
    });

    // Motor
    add_irq(motor.enc_left.A.pin, false, []() { motor.enc_left.callback_a_rise(); });
    add_irq(motor.enc_right.A.pin, false, []() { motor.enc_right.callback_a_rise(); });

    // Proxy sensor
    // add_irq(proxy.pin, false, []() { disable(); });
}

struct repeating_timer ultra_trig_up_timer;
struct repeating_timer ultra_trig_down_timer;
struct repeating_timer motor_interrupt_timer;

void register_timers() {
    { // Ultra sensor
        add_repeating_timer_ms(
            -60,
            [](__unused repeating_timer* t) {
                ultra.timer_callback_trig_up();
                return true;
            },
            NULL, &ultra_trig_up_timer);

        sleep_us(15);

        add_repeating_timer_ms(
            -60,
            [](__unused repeating_timer* t) {
                ultra.timer_callback_trig_down();
                return true;
            },
            NULL, &ultra_trig_down_timer);
    }

    { // Motor
        add_repeating_timer_ms(
            -5,
            [](__unused repeating_timer* t) {
                motor.timer_callback();
                return true;
            },
            NULL, &motor_interrupt_timer);
    }
}

void init() {
    stdio_init_all();
    cyw43_arch_init();

    register_irqs();
    register_timers();

    // we never deinit button
    button.init();

    // all other devices can be toggled with the button
    enable();

    servo.deg(0);
    rgb_sensor.led.set(0);
}

void enable() {
    if (bobot_enabled)
        return;

    bobot_enabled = true;

    motor.init();
    buzzer.init();
    proxy.init();
    ultra.init();
    servo.init();
    rgb_sensor.init();
    rgb_led.init();
    led.on();
}

void disable() {
    if (!bobot_enabled)
        return;

    bobot_enabled = false;

    motor.deinit();
    buzzer.deinit();
    proxy.deinit();
    ultra.deinit();
    servo.deinit();
    rgb_sensor.deinit();
    rgb_led.deinit();
    led.off();
}

void toggle() {
    if (bobot_enabled) {
        disable();
    } else {
        enable();
    }
}

void gpio_irq(uint gpio, uint32_t event_mask) {
    bool is_fall = event_mask & GPIO_IRQ_EDGE_FALL;

    // printf("%d %d\n", gpio, (int) is_fall);

    GpioIrq irq = irqs[is_fall][gpio];

    if (irq)
        irq();
}

} // namespace Bobot
