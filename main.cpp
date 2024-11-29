#include "pico/stdlib.h"
#include <stdio.h>

#include "ultra_sensor.h"

UltraSensor ultra = {20, 19};
volatile uint64_t last_pulse, last_dt;

bool trig_up(
    __unused struct repeating_timer* t) {

    ultra.trig.value(1);

    return true;
}

bool trig_down(
    __unused struct repeating_timer* t) {

    ultra.trig.value(0);

    return true;
}

void gpio_callback(uint gpio, uint32_t event_mask) {
        if (event_mask & GPIO_IRQ_EDGE_RISE) {
            last_pulse = time_us_64();
        }
        else if (event_mask & GPIO_IRQ_EDGE_FALL) {
            last_dt = time_us_64() - last_pulse;
        }
}

int main() {
    stdio_init_all();
    sleep_ms(7000);
    printf("Hello Timer!\n");

    struct repeating_timer up_timer;
    struct repeating_timer down_timer;

    add_repeating_timer_ms(-60, trig_up,
                           NULL, &up_timer);

    sleep_us(15);
    add_repeating_timer_ms(-60, trig_down,
                           NULL, &down_timer);


    gpio_set_irq_enabled_with_callback(
        ultra.echo.pin,
        GPIO_IRQ_EDGE_RISE,
        true,
        &gpio_callback        
    );
 
    // gpio_set_irq_enabled_with_callback(
    //     ultra.echo.pin,
    //     GPIO_IRQ_EDGE_FALL,
    //     true,
    //     &gpio_callback     
    // );

    while (true) {
        printf("dt: %d\n", last_pulse);
    }
}