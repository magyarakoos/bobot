#include "encoder.h"

#include "encoder.pio.h"
#include "pico/assert.h"

namespace {

struct LoadedProgram {
    PIO pio;
    uint offset;
};

constexpr size_t MAX_PIO_BLOCKS = 3; // pio0, pio1, pio2
LoadedProgram g_loaded[MAX_PIO_BLOCKS];
size_t g_loaded_count = 0;

void ensure_program_loaded(PIO pio) {
    for (size_t i = 0; i < g_loaded_count; ++i) {
        if (g_loaded[i].pio == pio) {
            return;
        }
    }

    hard_assert(g_loaded_count < MAX_PIO_BLOCKS);
    uint offset = pio_add_program(pio, &quadrature_encoder_program);
    g_loaded[g_loaded_count++] = LoadedProgram{ pio, offset };
}

} // namespace

Encoder::Encoder(uint a_pin, uint b_pin, PIO pio, int max_step_rate) : pio_(pio) {
    if (b_pin != a_pin + 1) {
        panic(
            "Encoder: PIO quadrature decoding requires b_pin == a_pin + 1 "
            "(the PIO reads both pins in a single instruction)");
    }

    ensure_program_loaded(pio_);

    sm_ = pio_claim_unused_sm(pio_, true);

    quadrature_encoder_program_init(pio_, sm_, a_pin, max_step_rate);
}

int32_t Encoder::ticks() const {
    return quadrature_encoder_get_count(pio_, sm_);
}
