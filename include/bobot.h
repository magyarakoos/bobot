#pragma once

#include <functional>
#include "onboard_led.h"

// wrapper for the peripheral drivers for Bobot
namespace Bobot {

// private bobot state variables
static volatile bool bobot_enabled = false;
static volatile uint64_t last_pause_us = 0;

// peripherals exposed to the user
extern OnboardLed led;

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
