#include "encoder.h"
#include "pico/stdlib.h"

Encoder::Encoder() : n(0), last_revolution(0), dt_sum(0) {}

float Encoder::get_rps() {
    if (last_revolution == 0)
        return 0;

    float dt_avg = (float) dt_sum / n;
    float dt = time_us_64() - last_revolution;
    float rps = 1e6 / TICKS_PER_REV / dt_avg;

    return (dt <= STOPPING_TRESHOLD_US) * rps;
}

/// RISE for both A and B
void Encoder::callback(volatile Encoder& e, Channel chan) {
    // e.dir = rise ^ ((chan == A) & ((e.chanState & B) == 0));

    // flip the appropriate channel
    e.chanState ^= chan;

    e.dir = e.chanState & Channel::A && !(e.chanState & Channel::B);

    if (e.dir)
        e.n++;
    else
        e.n--;

    /* if (chan && rise) { // chan A
        if (e.chanB)
            e.n--; // UNEREA
        else
            e.n++;
    } else if (!chan && rise) { // chan B
        if (chan)
            e.n++;
        else
            e.n--;
    }

    if (chan)
        e.chanA = !e.chanA;
    else
        e.chanB = !e.chanB; */

    /* e.n++;

    uint64_t now = time_us_64();
    e.dt_sum += now - e.last_revolution;
    e.last_revolution = now;

    if (e.n == e.TICKS_PER_REV) {
        e.dt_sum = 0;
        e.n = 0;
    } */
}
