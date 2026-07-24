#pragma once

#include <pico/types.h>

class Pin {
public:
    enum class Direction { In, Out };
    enum class Pull { None, Up, Down };

    Pin(uint gpio, Direction dir, Pull pull = Pull::None);

    bool value() const;
    void value(bool v);
    void on();
    void off();

    uint gpio() const { return gpio_; }

private:
    uint gpio_;
    Direction dir_;
};
