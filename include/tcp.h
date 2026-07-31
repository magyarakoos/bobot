#pragma once

#include <cstdint>
#include <functional>
#include "lwip/tcp.h"

enum TcpError {
    OK,
    INVALID_IP,
    PCB_FAILED,
    TCP_CONNECT_ERROR,
    CYW43_FAILED,
    WIFI_CONNECTION_FAILED,
    NO_MESSAGE,
    NOT_CONNECTED,
    NO_PCB,
    RX_BUFFER_FULL
};

class TcpClient {
    static constexpr uint32_t RX_BUFFER_SIZE = 4096;

    using PacketCallback = std::function<void(const uint8_t* payload, uint32_t length)>;

    static err_t on_recv_raw(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err);
    static err_t on_connected_raw(void* arg, struct tcp_pcb* tpcb, err_t err);
    static void on_err_raw(void* arg, err_t err);

    err_t handle_recv(struct tcp_pcb* tpcb, struct pbuf* p, err_t err);
    err_t handle_connected(struct tcp_pcb* tpcb, err_t err);
    void handle_err(err_t err);

    void process_rx_buffer();

    struct tcp_pcb* pcb_ = nullptr;
    uint8_t rx_buffer_[RX_BUFFER_SIZE];
    uint16_t rx_len_ = 0;
    bool connected_ = false;

    PacketCallback packet_callback_ = nullptr;

public:
    TcpClient();
    ~TcpClient();

    TcpClient(const TcpClient&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;

    TcpError init_wifi(const char* ssid, const char* password, uint32_t timeout_ms);

    TcpError connect(const char* ip, uint16_t port);
    void disconnect();

    TcpError send(const uint8_t* payload, uint32_t length);
    TcpError send(const char* message);

    bool is_connected() const { return connected_; }
    void set_packet_callback(PacketCallback callback) { packet_callback_ = callback; }
};
