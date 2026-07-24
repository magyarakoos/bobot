#include "ultra_sensor.h"
#include "gpio_irq.h"

UltraSensor::UltraSensor(uint trig_gpio, uint echo_gpio) : trig_(trig_gpio), echo_(echo_gpio) {
    gpio_init(trig_);
    gpio_set_dir(trig_, GPIO_OUT);
    gpio_put(trig_, false);

    gpio_init(echo_);
    gpio_set_dir(echo_, GPIO_IN);

    GpioIrq::attach(echo_, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, [this](uint gpio, uint32_t events) {
        uint32_t now = time_us_32();
        if (gpio_get(echo_)) {
            rise_ = now;
        } else {
            pulse_width_us_ = now - rise_;
            last_measurement_us_ = now;
        }
    });

    add_repeating_timer_ms(
        -TRIGGER_PERIOD_MS,
        [](__unused repeating_timer* t) {
            auto self = static_cast<UltraSensor*>(t->user_data);
            gpio_put(self->trig_, true);

            alarm_id_t id = add_alarm_in_us(
                TRIGGER_PULSE_US,
                [](__unused alarm_id_t id, void* data) -> int64_t {
                    auto self = static_cast<UltraSensor*>(data);
                    gpio_put(self->trig_, false);
                    return 0;
                },
                self, true);

            if (id < 0) {
                gpio_put(self->trig_, false);
            }
            return true;
        },
        this, &trigger_timer_);
}

float UltraSensor::dist() {
    uint32_t now = time_us_32();

    float d;
    if (now - last_measurement_us_ > TIME_WINDOW_US) {
        d = -1.0f;
    } else {
        d = pulse_width_us_ * CM_PER_US;
    }

    if (d < MIN_DIST || d > MAX_DIST) {
        return -1.0f;
    }
    return d;
}
