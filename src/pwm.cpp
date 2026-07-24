#include "pwm.h"

#include <pico/stdlib.h>
#include "hardware/clocks.h"
#include "hardware/pwm.h"

Pwm::Pwm(uint gpio, uint32_t freq_hz) : gpio_(gpio), wrap_(0) {
    gpio_set_function(gpio_, GPIO_FUNC_PWM);
    slice_ = pwm_gpio_to_slice_num(gpio_);
    channel_ = pwm_gpio_to_channel(gpio_);

    apply_freq(freq_hz);
    pwm_set_chan_level(slice_, channel_, 0);
    pwm_set_enabled(slice_, true);
}

void Pwm::apply_freq(uint32_t freq_hz) {
    if (freq_hz == 0) {
        freq_hz = 1;
    }

    uint32_t sys_hz = clock_get_hz(clk_sys);

    float clkdiv = 1.0f;
    uint32_t wrap = sys_hz / freq_hz;
    while (wrap > 65535 && clkdiv < 255.0f) {
        clkdiv += 1.0f;
        wrap = uint32_t(float(sys_hz) / (clkdiv * float(freq_hz)));
    }
    if (wrap > 0) {
        wrap -= 1;
    }

    pwm_set_clkdiv(slice_, clkdiv);
    pwm_set_wrap(slice_, wrap);
    wrap_ = wrap;
}

void Pwm::freq(uint32_t freq_hz) {
    apply_freq(freq_hz);
}

void Pwm::duty_u16(uint16_t duty) {
    uint32_t level = (uint32_t(duty) * wrap_) / 0xffff;
    pwm_set_chan_level(slice_, channel_, level);
}
