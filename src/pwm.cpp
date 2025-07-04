#include "pwm.h"
#include "hardware/pwm.h"
#include "utils.h"

void PWM::calculate_values() {

    // set the clock divider
    pwm_set_clkdiv(slice_num, CLKDIV);

    // wrap value
    // the CPU starts counting, and this will be the last value where it is still going
    // with the frequency, we set how many times a full cycle will be executed every second
    uint wrap = (CLOCK_SPEED / CLKDIV / (float) frequency);

    // level value
    // this can range from 0 to wrap
    // after the CPU reaches level, it switches from high to low
    uint level = duty_cycle * wrap;

    // configure the PWM slice for the desired frequency (wrap value)
    pwm_set_wrap(slice_num, wrap);

    // set the duty cycle (level value)
    pwm_set_chan_level(slice_num, channel_num, level);
}

PWM::PWM(uint _pin)
    : pin(_pin),
      frequency(0),
      duty_cycle(0),
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

    // The frequency cannot be lower than this value, otherwise wrap would overflow
    // And it also cannot be higher than this value, since that is the maximum
    // number of integer steps
    frequency = clamp(frequency, CLOCK_SPEED / CLKDIV / 0xffff, CLOCK_SPEED / CLKDIV);

    calculate_values();
}

void PWM::duty(float _duty_cycle) {
    duty_cycle = _duty_cycle;

    // duty cycle should be between 0 and 1
    duty_cycle = clamp(duty_cycle, 0.0f, 1.0f);

    calculate_values();
}
