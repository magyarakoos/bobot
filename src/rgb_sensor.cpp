#include "rgb_sensor.h"
#include <cstring>

RgbSensor::RgbSensor(uint sda_pin,
                     uint scl_pin,
                     uint i2c_index,
                     uint led_pin,
                     uint _integration_time,
                     uint _gain,
                     uint _address)
    : address(_address),
      i2c(sda_pin, scl_pin, i2c_index),
      inited(false),
      integration_time(_integration_time),
      gain(_gain),
      led(led_pin) {}

void RgbSensor::init() {
    if (inited)
        return;

    inited = true;

    i2c.init();
    led.init();

    sleep_ms(100);
    write8(REG_ENABLE, PON);
    sleep_ms(100);
    write_bits(REG_ENABLE, AEN, AEN);

    sleep_ms(100);
    set_integration_time(integration_time);
    set_gain(gain);
}

void RgbSensor::deinit() {

    if (!inited)
        return;

    inited = false;

    i2c.deinit();
    led.deinit();
}

void RgbSensor::write8(uint8_t reg, uint8_t value) {
    uint8_t buf[1] = { value };
    i2c.write(address, CMD_BIT | reg, buf, 1);
}

uint8_t RgbSensor::read8(uint8_t reg) {
    uint8_t buf[1] = { 0 };
    i2c.read(address, CMD_BIT | reg, buf, 1);
    return buf[0];
}

void RgbSensor::write_bits(uint8_t reg, uint8_t value, uint8_t mask) {
    uint8_t old = read8(reg);
    uint8_t old_masked = old & ~mask;
    uint8_t new_value = old_masked | (value & mask);

    write8(reg, new_value);
}

std::array<uint16_t, 4> RgbSensor::get_data() {
    std::array<uint16_t, 4> values;
    i2c.read(address, CMD_BIT | REG_CDATAL, (uint8_t*) values.data(), 4 * 2);
    return values;
}

Color RgbSensor::measure() {
    auto [c, r, g, b] = get_data();
    return { c, r, g, b };
}

void RgbSensor::set_integration_time(uint _integration_time) {
    integration_time = _integration_time;
    write8(REG_ATIME, 0xFF - integration_time);
}

void RgbSensor::set_gain(uint _gain) {
    gain = _gain;
    write_bits(REG_CONTROL, gain, 0b11);
}
