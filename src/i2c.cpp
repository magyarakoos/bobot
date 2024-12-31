#include "i2c.h"

I2C::I2C(uint _sda_pin, uint _scl_pin, uint _i2c_index = i2c_get_index(i2c_default))
    : sda_pin(_sda_pin), scl_pin(_scl_pin), i2c_index(_i2c_index) {
    enable();
}

I2C::~I2C() {
    disable();
}

void I2C::enable() {
    i2c_init(i2c_get_instance(i2c_index), 100'000); // 100 KHz
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
}

void I2C::disable() {
    i2c_deinit(i2c_get_instance(i2c_index));
    gpio_deinit(sda_pin);
    gpio_deinit(scl_pin);
}

uint8_t* I2C::read(uint8_t addr, uint8_t memaddr, uint nbytes) {
    uint8_t buf[nbytes] = { 0 };
    i2c_read_blocking(i2c_get_instance(i2c_index), addr, buf, nbytes, false);
    return buf;
}

void I2C::write(uint8_t addr, uint8_t memaddr, uint8_t* buf, uint8_t nbytes) {
    i2c_write_blocking(i2c_get_instance(i2c_index), addr, buf, nbytes, false);
}
