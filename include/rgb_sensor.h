#pragma once

#include <stdint.h>
#include <array>
#include <string.h>
#include "i2c.h"
#include "pico/stdlib.h"
#include "pin.h"

class RgbSensor {

    static const uint8_t DEFAULT_ADDRESS = 0x29;
    static const uint8_t CMD_BIT = 0x80;

    static const uint8_t REG_ENABLE = 0x00;
    static const uint8_t REG_CDATAL = 0x14;
    static const uint8_t REG_CONTROL = 0x0F;
    static const uint8_t REG_ATIME = 0x01;

    static const uint8_t PON = 0x01;
    static const uint8_t AEN = 0x02;

    uint8_t address;
    I2C i2c;

    void write8(uint8_t reg, uint8_t value);

    uint8_t read8(uint8_t reg);

    void write_bits(uint8_t reg, uint8_t value, uint8_t mask);

public:
    RgbSensor(uint sda_pin,
              uint scl_pin,
              uint i2c_address,
              uint led_pin,
              uint integration_time,
              uint gain,
              uint _address = DEFAULT_ADDRESS);

    Pin led;

    std::array<uint16_t, 4> get_data();

    void set_integration_time(uint integration_time);

    void set_gain(uint gain);
};
