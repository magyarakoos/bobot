#include "pin.h"
#include "pico/stdlib.h"

Pin::Pin(uint _pin, bool _out, bool _pull_up) : out(_out), _value(0), pull_up(_pull_up), inited(false), pin(_pin) {}

Pin::Pin(uint _pin, bool _out) : Pin(_pin, _out, false) {}

Pin::Pin(uint _pin) : Pin(_pin, GPIO_OUT) {}

void Pin::init() {
    if (inited)
        return;

    inited = true;

    gpio_init(pin);
    gpio_set_dir(pin, out);
    if (pull_up)
        gpio_pull_up(pin);
}

void Pin::deinit() {
    if (!inited)
        return;

    inited = false;

    gpio_deinit(pin);
}

bool Pin::value(bool __value) {
    if (!inited)
        return 0;

    _value = __value;
    if (out) {
        gpio_put(pin, _value);
        return 0;
    } else {
        return gpio_get(pin);
    }
}

void Pin::toggle() {
    value(!_value);
}
