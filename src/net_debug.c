#include "debug.h"
#include "net.h"

#include "net_config.h"

#include "pico/platform/panic.h"

#include "lwip/def.h"
#include "lwip/err.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/tcpbase.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static struct tcp_pcb* _net_debug_client_pcb = NULL;

// !!has to be larger than the largest expected piece of data, otherwise we have to panic!!

// ??????????????? uninitalized buffer
// uuu???????????? unparsed bytes arrive
// ppu???????????? parsed beginning
// u?????????????? popped parsed
// uu????????????? new bytes arrive

static uint8_t _net_debug_rec_buf[512];
static uint16_t _net_debug_rec_buf_count = 0;

static volatile bool _net_debug_connected = false;
static volatile uint8_t _net_debug_preconn_buf[0xff];
static volatile size_t _net_debug_preconn_buf_count = 0;

// DEBUG SERVER
static err_t _net_debug_connected_cb(void* arg, struct tcp_pcb* tpcb, err_t err) {
    (void) tpcb;
    (void) arg;

    _net_debug_connected = true;
    _net_debug_write((const void*) _net_debug_preconn_buf, _net_debug_preconn_buf_count);

    // shouldn't happen according to docs...
    if (err != ERR_OK) {
        printf("Connection failed with error: %d\n", err);
        return err;
    }

    puts("Connected to debug server");
    return ERR_OK;
}

static err_t __not_in_flash_func(_net_debug_recv_cb)(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err) {
    (void) arg;

    puts("in net debug receive callback...");

    if (p == NULL)
        return tcp_close(tpcb);

    if (err != ERR_OK)
        return err;

    if (p->tot_len <= 0) {
        pbuf_free(p);
        return ERR_OK;
    }

    printf("receiving %d bytes (+ %d in recv buffer)\n", (int) p->tot_len, (int) _net_debug_rec_buf_count);

    uint16_t total_read = 0;
    while (total_read < p->tot_len) {
        uint16_t read_from_pbuf = pbuf_copy_partial(
            p, _net_debug_rec_buf + _net_debug_rec_buf_count,
            LWIP_MIN(p->tot_len - total_read, sizeof(_net_debug_rec_buf) - _net_debug_rec_buf_count), //
            total_read);
        if (read_from_pbuf == 0)
            break;

        _net_debug_rec_buf_count += read_from_pbuf;
        total_read += read_from_pbuf;

        printf("read in %d bytes from pbuf to debug recv buf\n", (int) read_from_pbuf);

        // read in vars while we can
        uint16_t total_parsed = 0;
        while (true) {
            int32_t parsed = debug_parse_val_recv(_net_debug_rec_buf + total_parsed, //
                                                  _net_debug_rec_buf_count - total_parsed);

            // data isn't complete
            if (parsed == -1) {
                // fine, we still have more buffer space, we can read in more and process it later
                if (_net_debug_rec_buf_count < sizeof(_net_debug_rec_buf)) {
                    printf("partial parse waiting for more data...\n");
                } else {
                    // a single var is larger than the buffer, we can't do anything
                    // (even if we discard the buffer we don't know where in the stream we were)
                    const char err[] = "[ERROR] debug variable larger than receive buffer";
                    debug_printf(err);
                    panic(err);
                }

                break;
            } else if (parsed == -2) {
                // parsing error
                puts("parsing error...");
                break;
            }

            total_parsed += parsed;
            printf("parsed %d bytes\n", (int) parsed);
        }

        printf("parsed %d bytes total\n", (int) total_parsed);

        if (total_parsed > 0) {
            puts("moving back receive buffer\n");

            _net_debug_rec_buf_count -= total_parsed;

            // move back contents of buffer (ringbuffers would make parsing complicated and the data is small)
            // if the buffer looks like this: [pppuuuuu] where p is parsed and u is unparsed
            // ie. u is longer than p, then the regions overlap, therefore we have to use memmove
            memmove(_net_debug_rec_buf, _net_debug_rec_buf + total_parsed, _net_debug_rec_buf_count);
        }
    }

    printf("done receiving, parsing, %d left in recv buf\n", (int) _net_debug_rec_buf_count);

    tcp_recved(tpcb, total_read);
    pbuf_free(p);

    return ERR_OK;
}

bool _net_connect_debug_server(void) {
    ip_addr_t server_ip;
    ip4addr_aton(CONF_DEBUG_SERVER_IP, &server_ip);

    _net_debug_client_pcb = tcp_new_ip_type(IP_GET_TYPE(server_ip));
    if (!_net_debug_client_pcb) {
        puts("[ERROR] Failed to create debug server PCB");
        return false;
    }

    // tcp_sent(_debug_client_pcb, _debug_sent_cb);
    tcp_recv(_net_debug_client_pcb, _net_debug_recv_cb);
    tcp_err(_net_debug_client_pcb, _net_err_cb);
    tcp_arg(_net_debug_client_pcb, NULL);

    return tcp_connect(_net_debug_client_pcb, &server_ip, CONF_DEBUG_SERVER_PORT, _net_debug_connected_cb) == ERR_OK;
}

void _net_debug_write(const void* buf, size_t size) {
    if (!_net_debug_connected) {
        memcpy((void*) (_net_debug_preconn_buf + _net_debug_preconn_buf_count), buf, size);
        _net_debug_preconn_buf_count += size;
        return;
    }

    err_t err = tcp_write(_net_debug_client_pcb, buf, size, TCP_WRITE_FLAG_COPY);
    if (err != ERR_OK) {
        puts("[ERROR] Failed to send data to debug server because:");
        _net_err_cb(NULL, err);
    }

    err = tcp_output(_net_debug_client_pcb);
    if (err != ERR_OK) {
        puts("[ERROR] Failed to send data to debug server because:");
        _net_err_cb(NULL, err);
    }
}
