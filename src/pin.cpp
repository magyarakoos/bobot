#include "pin.h"
#include "pico/stdlib.h"

Pin::Pin(uint _pin, bool _out, bool _pull_up) : pin(_pin), out(_out), _value(0) {
    enable();

    if (_pull_up)
        gpio_pull_up(pin);
}

Pin::Pin(uint _pin, bool _out) : Pin(_pin, _out, false) {}

Pin::Pin(uint _pin) : Pin(_pin, GPIO_OUT) {}

void Pin::enable() {
    gpio_init(pin);
    gpio_set_dir(pin, out);
}

void Pin::disable() {
    gpio_deinit(pin);
}

Pin::~Pin() {
    disable();
}

bool Pin::value(bool __value) {
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
