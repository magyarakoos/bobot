#include "net.h"

#include "net_config.h"

#include "cyw43.h"
#include "pico/cyw43_arch.h"

#include <stdio.h>

static bool _net_init = false;

bool net_init(void) {
    if (net_isinit()) {
        puts("[WARNING] net already init");
        return false;
    }

    if (!cyw43_is_initialized(&cyw43_state)) {
        if (cyw43_arch_init()) {
            puts("[ERROR] Failed to init network");
            return false;
        }
    } else
        puts("[INFO] cyw43 already inited outside net, skipping");

    cyw43_arch_enable_sta_mode();

    puts("Net Init Egy");

    if (cyw43_arch_wifi_connect_blocking(CONF_WIFI_SSID, CONF_WIFI_PASSW, CONF_WIFI_AUTH)) {
        puts("[ERROR] Failed to connect to wifi");
        cyw43_arch_deinit();
        return false;
    }
    puts("Net Init Ketto");

    _net_init = true;

    puts("Net Init Harom");

    return true;
}

void net_poll(void) {
    cyw43_arch_poll();
}

void __not_in_flash_func(_net_err_cb)(void* arg, err_t err) {
    char* err_str;

    switch (err) {
        case ERR_OK: err_str = "No error, everything OK"; break;
        case ERR_MEM: err_str = "Out of memory error"; break;
        case ERR_BUF: err_str = "Buffer error"; break;
        case ERR_TIMEOUT: err_str = "Timeout"; break;
        case ERR_RTE: err_str = "Routing problem"; break;
        case ERR_INPROGRESS: err_str = "Operation in progress"; break;
        case ERR_VAL: err_str = "Illegal value"; break;
        case ERR_WOULDBLOCK: err_str = "Operation would block"; break;
        case ERR_USE: err_str = "Address in use"; break;
        case ERR_ALREADY: err_str = "Already connecting"; break;
        case ERR_ISCONN: err_str = "Conn already established"; break;
        case ERR_CONN: err_str = "Not connected"; break;
        case ERR_IF: err_str = "Low-level netif error"; break;
        case ERR_ABRT: err_str = "Connection aborted"; break;
        case ERR_RST: err_str = "Connection reset"; break;
        case ERR_CLSD: err_str = "Connection closed"; break;
        case ERR_ARG: err_str = "Illegal argument"; break;

        default: err_str = "unknown error";
    }

    printf("[ERROR] lwip error: %s", err_str);
}

bool net_isinit(void) {
    return _net_init;
}
