#include "tcp.h"
#include <cstdio>
#include <cstring>
#include "lwip/def.h"
#include "pico/cyw43_arch.h"

TcpClient::TcpClient() = default;

TcpClient::~TcpClient() {
    disconnect();
}

TcpError TcpClient::init_wifi(const char* ssid, const char* password, uint32_t timeout_ms) {
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_HUNGARY)) {
        printf("[TCP] Failed to initialize cyw43\n");
        return TcpError::CYW43_FAILED;
    }

    cyw43_arch_enable_sta_mode();
    cyw43_wifi_pm(&cyw43_state, CYW43_PERFORMANCE_PM);
    printf("[TCP] Connecting to Wi-Fi '%s'...\n", ssid);

    auto err = cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA3_WPA2_AES_PSK, timeout_ms);
    if (err) {
        printf("[TCP] Wi-Fi connection failed: %d\n", err);
        return TcpError::WIFI_CONNECTION_FAILED;
    }

    printf("[TCP] Wi-Fi Connected!\n");
    return TcpError::OK;
}

TcpError TcpClient::connect(const char* ip, uint16_t port) {
    if (connected_) {
        disconnect();
    }

    ip_addr_t server_ip;
    if (!ipaddr_aton(ip, &server_ip)) {
        printf("[TCP] Invalid IP address: %s\n", ip);
        return TcpError::INVALID_IP;
    }

    pcb_ = tcp_new_ip_type(IPADDR_TYPE_V4);
    if (!pcb_) {
        printf("[TCP] Failed to create PCB\n");
        return TcpError::PCB_FAILED;
    }

    tcp_arg(pcb_, this);
    tcp_err(pcb_, on_err_raw);
    tcp_recv(pcb_, on_recv_raw);

    cyw43_arch_lwip_begin();
    err_t err = tcp_connect(pcb_, &server_ip, port, on_connected_raw);
    cyw43_arch_lwip_end();

    if (err != ERR_OK) {
        printf("[TCP] tcp_connect error: %d\n", err);
        pcb_ = nullptr;
        return TcpError::TCP_CONNECT_ERROR;
    }

    return TcpError::OK;
}

void TcpClient::disconnect() {
    if (pcb_) {
        cyw43_arch_lwip_begin();
        tcp_arg(pcb_, nullptr);
        tcp_err(pcb_, nullptr);
        tcp_recv(pcb_, nullptr);
        tcp_close(pcb_);
        cyw43_arch_lwip_end();
        pcb_ = nullptr;
    }
    connected_ = false;
    rx_len_ = 0;
}

TcpError TcpClient::send(const char* message) {
    if (!message) {
        printf("[TCP] No message, dropping packet\n");
        return TcpError::NO_MESSAGE;
    }
    return send(reinterpret_cast<const uint8_t*>(message), strlen(message));
}

TcpError TcpClient::send(const uint8_t* payload, uint32_t length) {
    if (!connected_) {
        printf("[TCP] Not connected, dropping packet\n");
        return TcpError::NOT_CONNECTED;
    }
    if (!pcb_) {
        printf("[TCP] No pcb, dropping packet\n");
        return TcpError::NO_PCB;
    }

    uint32_t total_len = 4 + length;

    cyw43_arch_lwip_begin();

    if (tcp_sndbuf(pcb_) < total_len) {
        printf("[TCP] TX buffer full, dropping packet\n");
        cyw43_arch_lwip_end();
        return TcpError::RX_BUFFER_FULL;
    }

    uint8_t header[4];
    uint32_t net_len = lwip_htonl(length);
    memcpy(header, &net_len, 4);

    tcp_write(pcb_, header, 4, TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE);
    tcp_write(pcb_, payload, length, TCP_WRITE_FLAG_COPY);
    tcp_output(pcb_);

    cyw43_arch_lwip_end();
    return TcpError::OK;
}

err_t TcpClient::on_recv_raw(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err) {
    return arg ? static_cast<TcpClient*>(arg)->handle_recv(tpcb, p, err) : ERR_OK;
}

err_t TcpClient::on_connected_raw(void* arg, struct tcp_pcb* tpcb, err_t err) {
    return arg ? static_cast<TcpClient*>(arg)->handle_connected(tpcb, err) : err;
}

void TcpClient::on_err_raw(void* arg, err_t err) {
    if (arg) {
        static_cast<TcpClient*>(arg)->handle_err(err);
    }
}

err_t TcpClient::handle_recv(struct tcp_pcb* tpcb, struct pbuf* p, err_t err) {
    if (!p) {
        printf("[TCP] Connection closed by server\n");
        connected_ = false;
        pcb_ = nullptr;
        tcp_close(tpcb);
        return ERR_OK;
    }

    tcp_recved(tpcb, p->tot_len);

    if (rx_len_ + p->tot_len <= RX_BUFFER_SIZE) {
        pbuf_copy_partial(p, rx_buffer_ + rx_len_, p->tot_len, 0);
        rx_len_ += p->tot_len;
    } else {
        printf("[TCP] RX buffer overflow!\n");
    }
    pbuf_free(p);

    process_rx_buffer();
    return ERR_OK;
}

err_t TcpClient::handle_connected(struct tcp_pcb* tpcb, err_t err) {
    if (err != ERR_OK) {
        printf("[TCP] Connection failed: %d\n", err);
        connected_ = false;
        return err;
    }

    printf("[TCP] Connected to server!\n");
    connected_ = true;
    return ERR_OK;
}

void TcpClient::handle_err(err_t err) {
    printf("[TCP] Fatal TCP error %d\n", err);
    connected_ = false;
    pcb_ = nullptr;
}

void TcpClient::process_rx_buffer() {
    while (rx_len_ >= 4) {
        uint32_t payload_len;
        memcpy(&payload_len, rx_buffer_, 4);
        payload_len = lwip_ntohl(payload_len);

        uint32_t frame_len = 4 + payload_len;

        if (rx_len_ >= frame_len) {
            if (packet_callback_) {
                packet_callback_(rx_buffer_ + 4, payload_len);
            }

            rx_len_ -= frame_len;
            if (rx_len_ > 0) {
                memmove(rx_buffer_, rx_buffer_ + frame_len, rx_len_);
            }
        } else {
            break;
        }
    }
}
