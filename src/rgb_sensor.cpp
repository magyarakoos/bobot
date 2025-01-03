#include "rgb_sensor.h"

RgbSensor::RgbSensor(uint sda_pin,
                     uint scl_pin,
                     uint i2c_index,
                     uint led_pin,
                     uint integration_time,
                     uint gain,
                     uint _address)
    : i2c(sda_pin, scl_pin, i2c_index), address(_address), led(led_pin) {

    write_bits(REG_ENABLE, PON, PON);
    sleep_ms(10);
    write_bits(REG_ENABLE, AEN, AEN);

    set_integration_time(integration_time);
    set_gain(gain);
}

void RgbSensor::write8(uint8_t reg, uint8_t value) {
    uint8_t buf[] = { value };
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
    uint8_t new_value = old_masked | value & mask;

    write8(reg, new_value);
}

std::array<uint16_t, 4> RgbSensor::get_data() {
    uint8_t color_bytes[4 * 2] = { 0 };
    i2c.read(address, CMD_BIT | REG_CDATAL, color_bytes, 4 * 2);

    std::array<uint16_t, 4> values;
    std::memcpy(values.data(), color_bytes, 4 * 2);

    return values;
}

void RgbSensor::set_integration_time(uint integration_time) {
    write8(REG_ATIME, 0xFF - integration_time);
}

void RgbSensor::set_gain(uint gain) {
    write_bits(REG_CONTROL, gain, 0b11);
}
