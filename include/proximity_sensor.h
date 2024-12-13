#pragma once

class ProximitySensor {
private:
public:
    uint pin;
    void (*handler)();

    ProximitySensor(uint pin, void (*fn)());
    ~ProximitySensor();
};
