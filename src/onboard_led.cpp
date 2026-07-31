#include "onboard_led.h"
#include "pico/cyw43_arch.h"

void OnboardLed::on() {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
}

void OnboardLed::off() {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
}
