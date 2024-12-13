#include "pwm.h"
#include <algorithm>

void PWM::calculate_values() {

    // calculate wrap and level
    uint wrap = (CLOCK_SPEED / (float) frequency) - 1;

    // configure the PWM slice for the desired frequency
    // (wrap value)
    pwm_set_wrap(slice_num, wrap);

    // set the duty cycle (level value)
    pwm_set_chan_level(slice_num, channel_num, level);
}

PWM::PWM(uint _pin)
    : pin(_pin),
      frequency(0),
      level(0),
      slice_num(pwm_gpio_to_slice_num(_pin)),
      channel_num(pwm_gpio_to_channel(_pin)) {
    enable();
}

PWM::~PWM() {
    disable();
}

void PWM::enable() {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    pwm_set_enabled(slice_num, true);
}

void PWM::disable() {
    gpio_deinit(pin);
}

void PWM::freq(uint _frequency) {
    frequency = _frequency;

    // if we want a lower frequency, we'd need to increase
    // the clock divider which introduces a ton of
    // complexity
    // TODO: solve this problem despite its complexity
    frequency = std::clamp(frequency, 1907u, CLOCK_SPEED);

    calculate_values();
}

void PWM::duty(float duty_cycle) {
    uint wrap = (CLOCK_SPEED / (float) frequency) - 1;
    uint level = duty_cycle * (wrap + 1);
    calculate_values();
}

void PWM::duty(uint16_t level) {
    this->level = level;
    calculate_values();
}
