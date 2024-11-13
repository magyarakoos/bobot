#include "onboard_led.h"
#include "pico/cyw43_arch.h"

OnboardLed::OnboardLed() : led_pin(CYW43_WL_GPIO_LED_PIN) {
    cyw43_arch_init();
}

void OnboardLed::on() {
    value = 1;
    cyw43_arch_gpio_put(led_pin, 1);
}

void OnboardLed::off() {
    value = 0;
    cyw43_arch_gpio_put(led_pin, 0);
}

void OnboardLed::toggle() {
    value ? off() : on();
}
