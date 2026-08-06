#include "rgb_sensor.h"
#include "pico/stdlib.h"
#include <cstring>
#include <utils.h>

RgbSensor::RgbSensor(uint sda_pin,
                      uint scl_pin,
                      uint i2c_index,
                      uint led_pin,
                      uint _integration_time,
                      uint _gain,
                      uint _address)
    : address(_address),
      i2c(i2c_get_instance(i2c_index)),
      integration_time(_integration_time),
      gain(_gain),
      led(led_pin, Pin::Direction::Out) {

    const uint I2C_FREQ = 100'000; // 100 kHz
    i2c_init(i2c, I2C_FREQ);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);

    write8(REG_ENABLE, PON);
    sleep_ms(15);
    write_bits(REG_ENABLE, AEN, AEN);

    set_integration_time(integration_time);
    set_gain(gain);
}

void RgbSensor::write8(uint8_t reg, uint8_t value) {
    uint8_t cmd = CMD_BIT | reg;
    uint8_t buf[2] = { cmd, value };
    i2c_write_blocking(i2c, address, buf, sizeof(buf), false);
}

uint8_t RgbSensor::read8(uint8_t reg) {
    uint8_t cmd = CMD_BIT | reg;
    i2c_write_blocking(i2c, address, &cmd, 1, true);
    uint8_t value = 0;
    i2c_read_blocking(i2c, address, &value, 1, false);
    return value;
}

void RgbSensor::read_block(uint8_t reg, uint8_t* buf, uint nbytes) {
    uint8_t cmd = CMD_BIT | AUTO_INCREMENT | reg;
    i2c_write_blocking(i2c, address, &cmd, 1, true);
    i2c_read_blocking(i2c, address, buf, nbytes, false);
}

void RgbSensor::write_bits(uint8_t reg, uint8_t value, uint8_t mask) {
    uint8_t old = read8(reg);
    uint8_t old_masked = old & ~mask;
    uint8_t new_value = old_masked | (value & mask);
    write8(reg, new_value);
}

std::array<uint16_t, 4> RgbSensor::get_data() {
    uint8_t raw[8] = {0};
    read_block(REG_CDATAL, raw, sizeof(raw));

    std::array<uint16_t, 4> values;
    values[0] = static_cast<uint16_t>(raw[0]) | (static_cast<uint16_t>(raw[1]) << 8);
    values[1] = static_cast<uint16_t>(raw[2]) | (static_cast<uint16_t>(raw[3]) << 8);
    values[2] = static_cast<uint16_t>(raw[4]) | (static_cast<uint16_t>(raw[5]) << 8);
    values[3] = static_cast<uint16_t>(raw[6]) | (static_cast<uint16_t>(raw[7]) << 8);
    return values;
}

Color RgbSensor::measure() {
    auto [c, r, g, b] = get_data();
    return { c, r, g, b };
}

void RgbSensor::set_integration_time(uint _integration_time) {
    integration_time = _integration_time;
    write8(REG_ATIME, clamp(0xFF - int(integration_time / 2.4f + 0.5f), 0, 0xFF));
}

void RgbSensor::set_gain(uint _gain) {
    gain = _gain;
    write_bits(REG_CONTROL, static_cast<uint8_t>(gain), 0b11);
}