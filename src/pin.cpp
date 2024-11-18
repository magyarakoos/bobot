#include "pin.h"
#include "pico/stdlib.h"

Pin::Pin(uint _pin, bool _out) : pin(_pin), out(_out), _value(0) {
    enable();
}

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

void Pin::toggle() { value(!_value); }
