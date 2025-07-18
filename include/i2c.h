#pragma once

#include "hardware/i2c.h"

class I2C {
    uint sda_pin;
    uint scl_pin;
    uint i2c_index;

    volatile bool inited;

public:
    I2C(uint _sda_pin, uint _scl_pin, uint i2c_index = i2c_get_index(i2c_default));

    void init();
    void deinit();

    void read(uint8_t addr, uint8_t memaddr, uint8_t* buf, uint nbytes);

    void write(uint8_t addr, uint8_t memaddr, const uint8_t* buf, uint8_t nbytes);
};
