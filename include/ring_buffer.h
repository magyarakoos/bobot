#pragma once

#include <pico/stdlib.h>

template <typename T, uint N>
class RingBuffer {
    T data[N] = {};
    int head;
    int tail;
    uint _size;

public:
    RingBuffer() : head(0), tail(0), _size(0) {}

    const T& front() const;
    const T& back() const;

    void push_front(const T& x);
    void push_back(const T& x);

    void pop_front();
    void pop_back();

    uint size() const;

    bool empty() const;
};
