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
#include <stdio.h>
#include <string.h>

static struct tcp_pcb* _net_debug_client_pcb = NULL;

static volatile u8_t _net_debug_tx_buf[32 * 1024];
static volatile u16_t _net_debug_tx_head = 0;
static volatile u16_t _net_debug_tx_tail = 0;
static volatile u16_t _net_debug_tx_send = 0;

static volatile bool _net_debug_connected = false;

err_t __not_in_flash_func(_net_debug_flush_tx_buf)(void) {
    u16_t head = _net_debug_tx_head;
    u16_t tail = _net_debug_tx_tail;
    u16_t send = _net_debug_tx_send;

    err_t err = ERR_OK;

    while (send != head) {
        if (tcp_sndbuf(_net_debug_client_pcb) <= 0 || tcp_sndqueuelen(_net_debug_client_pcb) >= TCP_SND_QUEUELEN)
            break;

        u16_t cnt;
        if (head > send)
            cnt = head - send;
        else
            cnt = sizeof(_net_debug_tx_buf) - send;

        cnt = LWIP_MIN(tcp_sndbuf(_net_debug_client_pcb), cnt);

        err = tcp_write(
            _net_debug_client_pcb,         //
            _net_debug_tx_buf + send, cnt, //
            0
        );

        if (err != ERR_OK)
            break;

        // send += cnt;
        // if (send == sizeof(_net_debug_tx_buf))
        //     send = 0;
        send = (send + cnt) % sizeof(_net_debug_tx_buf);

        // printf("tx send: %d, \n", (int) _net_debug_tx_send);

        err = tcp_output(_net_debug_client_pcb);

        if (err != ERR_OK)
            break;
    }

    _net_debug_tx_send = send;

    return err;
}

err_t __not_in_flash_func(_net_debug_sent_cb)(void* arg, struct tcp_pcb* tpcb, u16_t len) {
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(tpcb);

    _net_debug_tx_tail = (_net_debug_tx_tail + len) % sizeof(_net_debug_tx_buf);
    // printf("tx tail: %d\n", (int) _net_debug_tx_tail);

    return _net_debug_flush_tx_buf();
}

static err_t __not_in_flash_func(_net_debug_recv_cb)(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err) {
    LWIP_UNUSED_ARG(arg);

    // !!has to be larger than the largest expected piece of data, otherwise we have to panic!!

    // ??????????????? uninitalized buffer
    // uuu???????????? unparsed bytes arrive
    // ppu???????????? parsed beginning
    // u?????????????? popped parsed
    // uu????????????? new bytes arrive
    static u8_t _net_debug_rx_buf[1024];
    static u16_t _net_debug_rx_buf_count = 0;

    if (p == NULL)
        return tcp_close(tpcb);

    if (err != ERR_OK)
        return err;

    if (p->tot_len <= 0) {
        pbuf_free(p);
        return ERR_OK;
    }

    // printf("receiving %d bytes (+ %d in recv buffer)\n", (int) p->tot_len, (int) _net_debug_rx_buf_count);

    u16_t total_read = 0;
    while (total_read < p->tot_len) {
        u16_t read_from_pbuf = pbuf_copy_partial(
            p, _net_debug_rx_buf + _net_debug_rx_buf_count,
            LWIP_MIN(p->tot_len - total_read, sizeof(_net_debug_rx_buf) - _net_debug_rx_buf_count), //
            total_read
        );
        if (read_from_pbuf == 0)
            break;

        _net_debug_rx_buf_count += read_from_pbuf;
        total_read += read_from_pbuf;

        // printf("read in %d bytes from pbuf to debug recv buf\n", (int) read_from_pbuf);

        // read in vars while we can
        u16_t total_parsed = 0;
        while (true) {
            int32_t parsed = debug_parse_val_recv(
                _net_debug_rx_buf + total_parsed, //
                _net_debug_rx_buf_count - total_parsed
            );

            // data isn't complete
            if (parsed == -1) {
                // fine, we still have more buffer space, we can read in more and process it later
                if (_net_debug_rx_buf_count < sizeof(_net_debug_rx_buf)) {
                    // printf("partial parse waiting for more data...\n");
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
                puts("[ERROR] debug variable parsing error...");
                break;
            }

            total_parsed += parsed;
            // printf("parsed %d bytes\n", (int) parsed);
        }
        // printf("parsed %d bytes total\n", (int) total_parsed);

        if (total_parsed > 0) {
            // puts("moving back receive buffer\n");

            _net_debug_rx_buf_count -= total_parsed;

            // move back contents of buffer (ringbuffers would make parsing complicated and the data is small)
            // if the buffer looks like this: [pppuuuuu] where p is parsed and u is unparsed
            // ie. u is longer than p, then the regions overlap, therefore we have to use memmove
            memmove(_net_debug_rx_buf, _net_debug_rx_buf + total_parsed, _net_debug_rx_buf_count);
        }
    }

    // printf("done receiving, parsing, %d left in recv buf\n", (int) _net_debug_rx_buf_count);

    tcp_recved(tpcb, total_read);
    pbuf_free(p);

    return _net_debug_flush_tx_buf();
}

static err_t __not_in_flash_func(_net_debug_poll_cb)(void* arg, struct tcp_pcb* tpcb) {
    return _net_debug_flush_tx_buf();
}

static err_t __not_in_flash_func(_net_debug_connected_cb)(void* arg, struct tcp_pcb* tpcb, err_t err) {
    LWIP_UNUSED_ARG(tpcb);
    LWIP_UNUSED_ARG(arg);

    _net_debug_connected = true;
    tcp_arg(_net_debug_client_pcb, NULL);
    tcp_err(_net_debug_client_pcb, _net_err_cb);
    tcp_sent(_net_debug_client_pcb, _net_debug_sent_cb);
    tcp_recv(_net_debug_client_pcb, _net_debug_recv_cb);
    tcp_poll(_net_debug_client_pcb, _net_debug_poll_cb, 1);

    _net_debug_flush_tx_buf();

    // shouldn't happen according to docs...
    if (err != ERR_OK) {
        printf("Connection failed with error: %d\n", err);
        return err;
    }

    puts("Connected to debug server");

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

    tcp_nagle_disable(_net_debug_client_pcb);

    err_t err = tcp_connect(
        _net_debug_client_pcb,  //
        &server_ip,             //
        CONF_DEBUG_SERVER_PORT, //
        _net_debug_connected_cb
    );

    return err == ERR_OK;
}

// copies the data into the tx buffer
void __not_in_flash_func(_net_debug_write)(const void* buf, size_t size) {
    // printf("_net_debug_write(%d)\n", (int) size);

    if (size == 0)
        return;

    DEBUG_DISABLE_IRQ {
        u16_t head = _net_debug_tx_head;
        u16_t tail = _net_debug_tx_tail;

        u16_t size_avail;
        if (tail > head)
            size_avail = tail - head;
        else
            size_avail = sizeof(_net_debug_tx_buf) - head + tail;

        if (size > size_avail) {
            puts("[ERROR] can't write to debug server, send buffer is full...");
            break;
        }

        while (size > 0) {
            // bool full = false;
            // if (head < tail)
            //     full = head == tail - 1;
            // else
            //     full = head == 0 && tail == sizeof(tx_buf) - 1;

            // |----ht----|  or |t--------h|
            // u16_t next_head = (head + 1) % sizeof(_net_debug_tx_buf);
            // bool full = next_head == tail;
            //
            // if (full) {
            //     puts("[ERROR] can't write to debug server, send buffer is full...");
            //     // we still need to reenable interrupts
            //     break;
            // }

            u16_t cnt;
            // if (head > tail)
            //     // |--t----h:::|
            //     cnt = sizeof(_net_debug_tx_buf) - head;
            // else // |---h:::t--|
            //     cnt = tail - head - 1;
            if (tail > head)
                cnt = tail - head - 1; // leave one byte gap to avoid full == empty ambiguity
            else
                cnt = sizeof(_net_debug_tx_buf) - head;

            // prevent full == empty confusion
            if (tail == 0 && head == sizeof(_net_debug_tx_buf) - 1)
                cnt--;

            cnt = LWIP_MIN(cnt, size);

            MEMCPY(_net_debug_tx_buf + head, buf, cnt);

            // we wrap around the buffer
            head = (head + cnt) % sizeof(_net_debug_tx_buf);
            // printf("tx head: %d\n", (int) _net_debug_tx_head);

            size -= cnt;
            buf += cnt;
        }

        _net_debug_tx_head = head;
    }
}
