#ifndef NET_H
#define NET_H

#include "lwip/err.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

bool net_init(void);
bool net_isinit(void);

void net_poll(void);

bool _net_connect_debug_server(void);
void _net_debug_write(const void* buf, size_t size);

bool _net_connect_mapman_server(void);
void _net_mapman_write(const void* buf, size_t size);

void _net_err_cb(void* arg, err_t err);

#ifdef __cplusplus
}
#endif

#endif
