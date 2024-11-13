#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <iostream>

const uint LED_PIN = CYW43_WL_GPIO_LED_PIN;
const uint LED_DELAY_MS = 100;

int main() {
    cyw43_arch_init();
    stdio_init_all();
    for (int i = 0 ;; i++) {
        std::cout << "Hello, world! T = " << i << "\n";

        cyw43_arch_gpio_put(LED_PIN, 1);
        sleep_ms(LED_DELAY_MS);
        cyw43_arch_gpio_put(LED_PIN, 0);
        sleep_ms(LED_DELAY_MS);
    }
}
