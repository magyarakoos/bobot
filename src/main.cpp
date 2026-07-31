#include <pico/stdlib.h>
#include <string>
#include "bobot.h"

int main() {
    Bobot bobot;

    TcpError x = bobot.setup_tcp();

    if (x != TcpError::OK) {
        while (true) {
            printf("TCP setup failed\n");
            sleep_ms(1000);
        }
    }

    while (true) {
        bobot.tcp.send((std::to_string(bobot.ultra.dist()) + " cm").c_str());
        printf("%f\n", bobot.ultra.dist());
        sleep_ms(500);
    }
}
