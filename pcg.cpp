#include "pcg.h"

PCG::PCG(uint64_t seed, uint64_t seq) : state(seed + seq), inc((seq << 1u) | 1u) {
    next();  // discard the first output, as suggested in the PCG paper
}

uint32_t PCG::next() {
    uint64_t oldState = state;
    state = oldState * 6364136223846793005ULL + inc;
    uint32_t xorshifted = ((oldState >> 18u) ^ oldState) >> 27u;
    uint32_t rot = oldState >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

PCG::result_type PCG::operator()() {
    return next();
}
