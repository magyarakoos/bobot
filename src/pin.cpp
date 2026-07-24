#include "pin.h"

#include "hardware/gpio.h"

Pin::Pin(uint gpio, Direction dir, Pull pull) : gpio_(gpio), dir_(dir) {
    gpio_init(gpio_);
    gpio_set_dir(gpio_, dir_ == Direction::Out ? GPIO_OUT : GPIO_IN);

    switch (pull) {
        case Pull::Up: gpio_pull_up(gpio_); break;
        case Pull::Down: gpio_pull_down(gpio_); break;
        case Pull::None:
        default: gpio_disable_pulls(gpio_); break;
    }
}

bool Pin::value() const {
    return gpio_get(gpio_);
}

void Pin::value(bool v) {
    gpio_put(gpio_, v);
}

void Pin::on() {
    gpio_put(gpio_, true);
}

void Pin::off() {
    gpio_put(gpio_, false);
}
