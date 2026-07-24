#include "rgb_led.h"

RGBLed::RGBLed(uint r_gpio, uint g_gpio, uint b_gpio, uint32_t freq_hz)
    : r_(r_gpio, freq_hz), g_(g_gpio, freq_hz), b_(b_gpio, freq_hz) {
    set_color(0, 0, 0);
}

void RGBLed::set_color(uint8_t r, uint8_t g, uint8_t b) {
    r_.duty_u16((255 - r) * 0xffff / 255);
    g_.duty_u16((255 - g) * 0xffff / 255);
    b_.duty_u16((255 - b) * 0xffff / 255);
}
