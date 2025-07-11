#pragma once

#include <functional>
#include "encoder.h"
#include "h_bridge.h"
#include "onboard_led.h"
#include "pin.h"
#include "pwm.h"
#include "rgb_sensor.h"
#include "servo.h"
#include "ultra_sensor.h"

// wrapper for the peripheral drivers for Bobot
// it's particularly useful for the IRQ function
// since this way it can access everything
namespace Bobot {

// private bobot state variables
static volatile bool bobot_enabled = false;
static volatile uint64_t last_pause_us = 0;

// peripherals exposed to the user
extern Pin button;
extern UltraSensor ultra;
extern PWM buzzer;
extern HBridge hb;
extern OnboardLed led;
extern Servo servo;
extern Pin proxy;
extern RgbSensor rgb_sensor;
extern Encoder enc_left;
extern Encoder enc_right;

using GpioIrq = std::function<void()>;

// initialize Bobot functinonality
void init();

// enables all peripherals of Bobot
void enable();

// disables all peripherals of Bobot
void disable();

// toggles all peripherals of Bobot
void toggle();

// convenience function for setting up gpio irq callbacks
void add_irq(uint gpio, bool is_fall, GpioIrq callback);

// static gpio IRQ function, which gets called whenever
// an interrupt is triggered
// all gpio IRQ logic is defined here
static void gpio_irq(uint gpio, uint32_t event_mask);

void print(const char* fmt, ...);

} // namespace Bobot
