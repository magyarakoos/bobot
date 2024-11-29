#pragma once

#include "pin.h"
#include "pwm.h"
#include "h_bridge.h"
#include "ultra_sensor.h"
#include "onboard_led.h"

// wrapper for the peripheral drivers for Bobot
// it's particularly useful for the IRQ function
// since this way it can access everything
namespace Bobot {

    // TODO: replace these with a config file
    static const uint BUZZER_PIN = 4;
    static const uint BUTTON_PIN = 0;
    static const uint DEBOUNCE_INTERVAL_US = 5e5;

    // private bobot state variables
    static bool bobot_enabled = true;
    static uint64_t last_pause_us = 0;

    // peripherals exposed to the user
    extern Pin button;
    extern PWM buzzer;
    extern HBridge hb;
    extern UltraSensor ultra;
    extern OnboardLed led;

    // initialize Bobot functinonality
    void init();

    // enables all peripherals of Bobot
    void enable();

    // disables all peripherals of Bobot
    void disable();

    // toggles all peripherals of Bobot
    void toggle();

    // static gpio IRQ function, which gets called whenever
    // an interrupt is triggered
    // all gpio IRQ logic is defined here
    static void gpio_irq(uint gpio, uint32_t event_mask);
}
