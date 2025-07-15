#pragma once

#include <functional>
#include "encoder.h"
#include "h_bridge.h"
#include "motor.h"
#include "onboard_led.h"
#include "pin.h"
#include "pwm.h"
#include "rgb_sensor.h"
#include "servo.h"
#include "ultra_sensor.h"

// wrapper for the peripheral drivers for Bobot
namespace Bobot {

// private bobot state variables
static volatile bool bobot_enabled = false;
static volatile uint64_t last_pause_us = 0;

// peripherals exposed to the user
extern Pin button;
extern UltraSensor ultra;
extern PWM buzzer;
extern OnboardLed led;
extern Servo servo;
extern Pin proxy;
extern RgbSensor rgb_sensor;
extern Motor motor;

using GpioIrq = std::function<void()>;

// initialize Bobot functinonality
// the IRQ and repeating timer abstractions are defined here
void init();

// enables all peripherals of Bobot
void enable();

// disables all peripherals of Bobot
void disable();

// toggles all peripherals of Bobot
void toggle();

// static gpio IRQ function, which gets called whenever
// a gpio interrupt is triggered
static void gpio_irq(uint gpio, uint32_t event_mask);

} // namespace Bobot
