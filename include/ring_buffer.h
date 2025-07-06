#pragma once

#include <pico/stdlib.h>

// statically allocated double-ended queue implemented with a ring buffer
//
// WARNING: calling `front()` or `back()` on an empty buffer will give nonsensical values
//
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

    void clear();
};

template <typename T, uint N>
const T& RingBuffer<T, N>::front() const {
    return data[tail];
}

template <typename T, uint N>
const T& RingBuffer<T, N>::back() const {
    return data[head];
}

template <typename T, uint N>
void RingBuffer<T, N>::push_front(const T& x) {
    if (!empty()) {
        tail = (tail - 1 + N) % N;
        // buffer full, delete from back
        if (size() == N) {
            pop_back();
        }
    }
    data[tail] = x;
    _size++;
}

template <typename T, uint N>
void RingBuffer<T, N>::push_back(const T& x) {
    if (!empty()) {
        head = (head + 1) % N;
        // buffer full, delete from front
        if (size() == N) {
            pop_front();
        }
    }
    data[head] = x;
    _size++;
}

template <typename T, uint N>
void RingBuffer<T, N>::pop_front() {
    if (empty())
        return;

    if (head != tail) {
        tail = (tail + 1) % N;
    }

    _size--;
}

template <typename T, uint N>
void RingBuffer<T, N>::pop_back() {
    if (empty())
        return;

    if (head != tail) {
        head = (head - 1 + N) % N;
    }

    _size--;
}

template <typename T, uint N>
uint RingBuffer<T, N>::size() const {
    return _size;
}

template <typename T, uint N>
bool RingBuffer<T, N>::empty() const {
    return size() == 0;
}

template <typename T, uint N>
void RingBuffer<T, N>::clear() {
    head = tail = _size = 0;
}
