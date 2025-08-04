#include <stdio.h>
#include "bobot.h"

void servo_test() {
    Bobot::init();

    int pos = 0, dir = 1;
    while (true) {
        Bobot::servo.deg(pos);
        pos += dir;
        sleep_ms(5);
        printf("%d %d\n", pos, (int) dir);
        if (pos <= -90) {
            dir = 1;
        } else if (pos >= 90) {
            dir = -1;
        }
    }
}

void pid_distancing() {
    Bobot::init();

    PID pid(200, 0, 0, 0, 0);
    pid.set_sp(0.3f);

    while (true) {
        float d = Bobot::ultra.distance_smooth();

        float u;
        if (d < 0) {
            u = 0;
        } else {
            u = -pid.compute(d);
        }

        Bobot::motor.drive(u, u);
        printf("%f %f\n", u, d);

        sleep_ms(60);
    }
}

void drive_test() {
    Bobot::init();

    Bobot::motor.drive(100, 100);
    while (true) {
        printf("%f %f\n", Bobot::motor.enc_left.get_speed(), Bobot::motor.enc_right.get_speed());
        sleep_ms(20);
    }
}

void rgb_sensor_test() {
    Bobot::init();
    Bobot::rgb_sensor.led.set(1);

    while (true) {
        Color c = Bobot::rgb_sensor.measure();
        printf("%d %d %d %d %d\n", (int) time_us_64(), c.c, c.r, c.g, c.b);
        sleep_ms(100);
    }
}

int main() {
    drive_test();
}
