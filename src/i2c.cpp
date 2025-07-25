#include "i2c.h"
#include "pico/stdlib.h"

I2C::I2C(uint _sda_pin, uint _scl_pin, uint _i2c_index)
    : sda_pin(_sda_pin), scl_pin(_scl_pin), i2c_index(_i2c_index), inited(false) {}

void I2C::init() {
    if (inited)
        return;

    inited = true;

    const uint I2C_FREQ = 100'000; // 100 KHz
    i2c_init(i2c_get_instance(i2c_index), I2C_FREQ);

    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
}

void I2C::deinit() {
    if (!inited)
        return;

    inited = false;

    i2c_deinit(i2c_get_instance(i2c_index));
    gpio_deinit(sda_pin);
    gpio_deinit(scl_pin);
}

void I2C::read(uint8_t addr, uint8_t memaddr, uint8_t* buf, uint nbytes) {
    if (!inited)
        return;

    const uint8_t membuf[1] = { memaddr };
    i2c_write_burst_blocking(i2c_get_instance(i2c_index), addr, membuf, 1);

    i2c_read_burst_blocking(i2c_get_instance(i2c_index), addr, buf, nbytes);
}

void I2C::write(uint8_t addr, uint8_t memaddr, const uint8_t* buf, uint8_t nbytes) {
    if (!inited)
        return;

    const uint8_t membuf[1] = { memaddr };
    i2c_write_burst_blocking(i2c_get_instance(i2c_index), addr, membuf, 1);

    i2c_write_burst_blocking(i2c_get_instance(i2c_index), addr, buf, nbytes);
}
