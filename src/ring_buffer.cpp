#include "ring_buffer.h"

template <typename T, uint N>
const T& RingBuffer<T, N>::front() {
    return data[tail];
}

template <typename T, uint N>
const T& RingBuffer<T, N>::back() {
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
uint RingBuffer<T, N>::size() {
    return _size;
}

template <typename T, uint N>
bool RingBuffer<T, N>::empty() {
    return size() == 0;
}
