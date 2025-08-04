// THIS MODULES'S PUBLIC API IS ONLY SAFE TO CALL FROM THE MAIN LOOP

#include "debug.h"
#include "net.h"
#include "utils.h"

#include "pico/unique_id.h"

#include <inttypes.h>
#include <math.h>
#include <pico/stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// server -> pico:
//   - control updated
//     - u16: key len
//     - [u8]: key
//     - ?:  value
//
// pico -> server:
//   - on connect:
//     - u64: pico id
//
//   - 0x01 - debug log
//     - u16:  len
//     - [u8]: str
//
//   - 0x02 - define controls
//     - u8: count
//     - *for each control*
//       - u16: key len
//       - [u8]: key
//       - u8: value type
//       - ?: default value

typedef struct _debug_var {
    volatile void* ptr;
    debug_type type;
    char name[_DEBUG_VAR_MAX_NAME];
} _debug_var;

static _debug_var _debug_vars[256];
static size_t _debug_var_count = 0;
static volatile bool _debug_sent_vars = false;

static uint8_t _debug_send_buf[1024];

static _debug_var* _debug_get_var(const char* name) {
    for (size_t i = 0; i < _debug_var_count; i++)
        if (strcmp(name, _debug_vars[i].name) == 0)
            return &_debug_vars[i];

    return nullptr;
}

static void _debug_clamp_var(_debug_var* var) {
    switch (var->type.ty) {
        case DEBUG_BOOL:
            volatile bool* valb = var->ptr;
            DEBUG_DISABLE_IRQ {
                *valb = !!*valb;
            }
            break;

        case DEBUG_FLOAT:
            struct _debug_float_range* fdata = &var->type.float_range;
            if (!isfinite(fdata->min))
                fdata->min = 0;

            if (!isfinite(fdata->max))
                fdata->max = 0;

            fdata->min = CLAMP(fdata->min, 0, UINT8_MAX);
            fdata->max = CLAMP(fdata->max, fdata->min, UINT8_MAX);

            volatile float* valf = var->ptr;
            DEBUG_DISABLE_IRQ {
                float tmpf = *valf;
                if (!isfinite(tmpf))
                    tmpf = 0;

                *valf = CLAMP(tmpf, (float) fdata->min, (float) fdata->max);
            }
            break;

        case DEBUG_INT:
            struct _debug_int_data* data = &var->type.int_data;

            switch (data->ty) {
                case DEBUG_U8:
                    data->min = CLAMP(data->min, 0, UINT8_MAX);
                    data->max = CLAMP(data->max, data->min, UINT8_MAX);

                    volatile uint8_t* valu8 = var->ptr;
                    *valu8 = CLAMP(*valu8, (uint8_t) data->min, (uint8_t) data->max);
                    break;

                case DEBUG_U16:
                    data->min = CLAMP(data->min, 0, UINT16_MAX);
                    data->max = CLAMP(data->max, data->min, UINT16_MAX);

                    volatile uint16_t* valu16 = var->ptr;
                    *valu16 = CLAMP(*valu16, (uint16_t) data->min, (uint16_t) data->max);
                    break;

                case DEBUG_U32:
                    data->min = CLAMP(data->min, 0, UINT32_MAX);
                    data->max = CLAMP(data->max, data->min, UINT32_MAX);

                    volatile uint32_t* valu32 = var->ptr;
                    *valu32 = CLAMP(*valu32, (uint32_t) data->min, (uint32_t) data->max);
                    break;

                case DEBUG_I8:
                    data->min = CLAMP(data->min, INT8_MIN, INT8_MAX);
                    data->max = CLAMP(data->max, data->min, INT8_MAX);

                    volatile int8_t* vali8 = var->ptr;
                    *vali8 = CLAMP(*vali8, (int8_t) data->min, (int8_t) data->max);
                    break;

                case DEBUG_I16:
                    data->min = CLAMP(data->min, INT16_MIN, INT16_MAX);
                    data->max = CLAMP(data->max, data->min, INT16_MAX);

                    volatile int16_t* vali16 = var->ptr;
                    *vali16 = CLAMP(*vali16, (int16_t) data->min, (int16_t) data->max);
                    break;

                case DEBUG_I32:
                    data->min = CLAMP(data->min, INT32_MIN, INT32_MAX);
                    data->max = CLAMP(data->max, data->min, INT32_MAX);

                    volatile int32_t* vali32 = var->ptr;
                    *vali32 = CLAMP(*vali32, (int32_t) data->min, (int32_t) data->max);
                    break;
            }
            break;

        case DEBUG_COLOR:
            volatile hsv_color* valc = var->ptr;

            DEBUG_DISABLE_IRQ {
                hsv_color tmpc = *valc;
                if (!isfinite(tmpc.h))
                    tmpc.h = 0;
                if (!isfinite(tmpc.s))
                    tmpc.s = 0;
                if (!isfinite(tmpc.v))
                    tmpc.v = 0;

                valc->h = CLAMP(tmpc.h, 0.f, 360.f);
                valc->s = CLAMP(tmpc.s, 0.f, 1.f);
                valc->v = CLAMP(tmpc.v, 0.f, 1.f);
            }
            break;
    }
}

void debug_add_remote_var_fn(volatile void* ptr, const char* name, debug_type type) {
    _debug_var* dv = &_debug_vars[_debug_var_count];

    dv->ptr = ptr;
    dv->type = type;
    strncpy(dv->name, name, sizeof(dv->name));

    _debug_clamp_var(dv);

    printf("[INFO] added debug var \"%s\" type: ", dv->name);
    switch (type.ty) {
        case DEBUG_BOOL: //
            printf("bool, value: %s\n", (*(bool*) ptr) ? "true" : "false");
            break;

        case DEBUG_COLOR:
            hsv_color col = *(volatile hsv_color*) ptr;
            printf("color, value: (%.2f, %.2f, %.2f)\n", col.h, col.s, col.v);
            break;

        case DEBUG_FLOAT:
            printf("float, value: %.2f, min: %.2f, max: %.2f\n", //
                   *(volatile float*) ptr, type.float_range.min, type.float_range.max);
            break;

        case DEBUG_INT:
            int64_t val = 0;

            switch (type.int_data.ty) {
                case DEBUG_U8: val = *(volatile uint8_t*) ptr; break;
                case DEBUG_U16: val = *(volatile uint16_t*) ptr; break;
                case DEBUG_U32: val = *(volatile uint32_t*) ptr; break;

                case DEBUG_I8: val = *(volatile int8_t*) ptr; break;
                case DEBUG_I16: val = *(volatile int16_t*) ptr; break;
                case DEBUG_I32: val = *(volatile int32_t*) ptr; break;
            }

            printf( // portable print of int64_t (defined in inttypes.h)
                "int, value: %" PRId64 ", min: %" PRId64 ", max: %" PRId64 "\n", //
                val, type.int_data.min, type.int_data.max);

            break;
    }

    _debug_var_count++;
}

#define _WRITE_TO_BUFFER(buffer, counter_var, value)               \
    do {                                                           \
        memcpy((buffer) + (counter_var), &(value), sizeof(value)); \
        (counter_var) += sizeof(value);                            \
    } while (0)

void debug_send_remote_vars(void) {
    if (_debug_sent_vars) {
        debug_printf("[ERROR] remote vars already sent");
        return;
    }

    printf("Sending %d debug variables\n", (int) _debug_var_count);

    // 0x02 = send controls
    _debug_send_buf[0] = 0x02;
    size_t bytes_written = 1;

    // write control count
    uint16_t var_count = (uint16_t) _debug_var_count;
    _WRITE_TO_BUFFER(_debug_send_buf, bytes_written, var_count);

    // write each control
    for (size_t i = 0; i < _debug_var_count; i++) {
        _debug_var var = _debug_vars[i];

        // name length
        uint16_t name_len = strlen(var.name);
        _WRITE_TO_BUFFER(_debug_send_buf, bytes_written, name_len);

        // name
        // we use the value of name_len, not sizeof(name_len)
        memcpy(_debug_send_buf + bytes_written, var.name, name_len);
        bytes_written += name_len;

        printf("%d. [%d]\"%s\"\n", i, (int) name_len, var.name);

        // type
        uint8_t var_ty = (uint8_t) var.type.ty;
        _WRITE_TO_BUFFER(_debug_send_buf, bytes_written, var_ty);

        // value, min, max
        switch (var.type.ty) {
            case DEBUG_INT:
                int64_t val = 0;

                switch (var.type.int_data.ty) {
                    case DEBUG_U8: val = (int64_t) *(volatile uint8_t*) var.ptr; break;
                    case DEBUG_U16: val = (int64_t) *(volatile uint16_t*) var.ptr; break;
                    case DEBUG_U32: val = (int64_t) *(volatile uint32_t*) var.ptr; break;

                    case DEBUG_I8: val = (int64_t) *(volatile int8_t*) var.ptr; break;
                    case DEBUG_I16: val = (int64_t) *(volatile int16_t*) var.ptr; break;
                    case DEBUG_I32: val = (int64_t) *(volatile int32_t*) var.ptr; break;
                }

                _WRITE_TO_BUFFER(_debug_send_buf, bytes_written, val);
                _WRITE_TO_BUFFER(_debug_send_buf, bytes_written, var.type.int_data.min);
                _WRITE_TO_BUFFER(_debug_send_buf, bytes_written, var.type.int_data.max);
                break;

            case DEBUG_BOOL:
                bool val_bool = *(volatile bool*) var.ptr;
                uint8_t val_bool_u8 = val_bool ? 1 : 0;

                _WRITE_TO_BUFFER(_debug_send_buf, bytes_written, val_bool_u8);
                break;

            case DEBUG_FLOAT:
                float val_float = *(volatile float*) var.ptr;

                _WRITE_TO_BUFFER(_debug_send_buf, bytes_written, val_float);
                _WRITE_TO_BUFFER(_debug_send_buf, bytes_written, var.type.float_range.min);
                _WRITE_TO_BUFFER(_debug_send_buf, bytes_written, var.type.float_range.max);
                break;

            case DEBUG_COLOR:
                hsv_color val_hsv;

                // more than 32 bits
                DEBUG_DISABLE_IRQ {
                    val_hsv = *(volatile hsv_color*) var.ptr;
                }

                _WRITE_TO_BUFFER(_debug_send_buf, bytes_written, val_hsv);
                break;
        }

        printf("bytes written: %d\n", (int) bytes_written);
    }

    _net_debug_write(_debug_send_buf, bytes_written);

    _debug_sent_vars = true;
}

void debug_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    // !! offsetting because of the 0x01 !!
    int wci = vsnprintf((char*) (_debug_send_buf + 1 + sizeof(uint16_t)), //
                        sizeof(_debug_send_buf) - (1 + sizeof(uint16_t)), //
                        format, args);

    va_end(args);

    // error
    if (wci < 0)
        return;

    uint16_t wc = wci;

    // vsnprintf returns the non-truncated length
    if (wc > sizeof(_debug_send_buf) - 1)
        wc = sizeof(_debug_send_buf) - 1;

    // remove a single trailing newline
    if (_debug_send_buf[wc - 1] == '\n')
        wc--;

    // 0x01 = logging
    _debug_send_buf[0] = 0x01;
    // string size
    memcpy(_debug_send_buf + 1, &wc, sizeof(wc));

    // we write the whole buffer
    _net_debug_write(_debug_send_buf, wc + 1 + sizeof(uint16_t));

    // no newline, since we are mimicking printf
    // also don't write the 0x01
    printf("%.*s", wci, (char*) (_debug_send_buf + 1 + sizeof(uint16_t)));
}

static size_t _debug_get_var_wire_size(debug_type type) {
    switch (type.ty) {
        case DEBUG_BOOL: return sizeof(bool);
        case DEBUG_FLOAT: return sizeof(float);
        case DEBUG_INT: return sizeof(int64_t);
        case DEBUG_COLOR: return sizeof(hsv_color);
        default: return 0;
    }
}

int32_t debug_parse_val_recv(const uint8_t* buf, size_t buf_size) {
    static char name[_DEBUG_VAR_MAX_NAME];

    uint16_t bytes_read = 0;

    // name len + name + at least one byte (bool a)
    if (buf_size - bytes_read < sizeof(uint16_t) + 2)
        return -1;

    uint16_t name_len = *(uint16_t*) (buf + bytes_read);
    bytes_read += sizeof(name_len);

    if (name_len > _DEBUG_VAR_MAX_NAME)
        return -2;

    if (name_len >= buf_size - bytes_read)
        return -1;

    memcpy(name, buf + bytes_read, name_len);
    name[name_len] = '\0';
    bytes_read += name_len;

    _debug_var* var = _debug_get_var(name);
    if (var == nullptr) {
        printf("[ERROR] unkown debug variable received: \"%s\"\n", name);
        return -2;
    }

    // the size of the variable
    size_t sz = _debug_get_var_wire_size(var->type);
    if (sz == 0) {
        puts("\n[ERROR] unknown debug variable type....?");
        return -2;
    }

    printf("%." _UTIL_EXPAND_AND_QUOTE(_DEBUG_VAR_MAX_NAME) "s's size: %d \n", var->name, (int) sz);

    if (buf_size - bytes_read < sz) {
        printf("[ERROR] not enough space in read buf: %d\n", (int) (buf_size - bytes_read));
        return -1;
    }

    printf("new value for %s: ", var->name);

    switch (var->type.ty) {
        case DEBUG_INT:
            int64_t val = *(int64_t*) (buf + bytes_read);
            switch (var->type.int_data.ty) {
                case DEBUG_U8: *(volatile uint8_t*) var->ptr = (uint8_t) val; break;
                case DEBUG_U16: *(volatile uint16_t*) var->ptr = (uint16_t) val; break;
                case DEBUG_U32: *(volatile uint32_t*) var->ptr = (uint32_t) val; break;

                case DEBUG_I8: *(volatile int8_t*) var->ptr = (int8_t) val; break;
                case DEBUG_I16: *(volatile int16_t*) var->ptr = (int16_t) val; break;
                case DEBUG_I32: *(volatile int32_t*) var->ptr = (int32_t) val; break;
            }
            printf("%" PRId64 "\n", val);
            break;

        case DEBUG_FLOAT:
            *(volatile float*) var->ptr = *(float*) (buf + bytes_read);
            printf("%f\n", *(volatile float*) var->ptr);
            break;

        case DEBUG_BOOL:
            *(volatile bool*) var->ptr = *(bool*) (buf + bytes_read);
            printf("%s\n", (*(volatile bool*) var->ptr) ? "true" : "false");
            break;

        case DEBUG_COLOR:
            hsv_color col_src = *(hsv_color*) (buf + bytes_read);

            // more than 32 bits
            volatile hsv_color* col_dst = var->ptr;
            // DEBUG_DISABLE_IRQ {
            col_dst->h = col_src.h;
            col_dst->s = col_src.s;
            col_dst->v = col_src.v;
            // }

            hsv_color col_res = *col_dst;
            printf("(%f %f %f)\n", col_res.h, col_res.s, col_res.v);
            break;
    }

    bytes_read += sz;

    return bytes_read;
}

bool debug_connect_server(void) {
    if (!_net_connect_debug_server())
        return false;

    pico_unique_board_id_t id;
    pico_get_unique_board_id(&id);
    _net_debug_write(&id, sizeof(id));

    return true;
}
