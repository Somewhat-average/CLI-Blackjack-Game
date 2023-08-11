#pragma once
#include <cstdint>

class PCG {
private:
    uint64_t state;
    uint64_t inc;

public:
    using result_type = uint32_t;  // Define result type

    // Define min and max inline
    static constexpr result_type min() { return 0; }  
    static constexpr result_type max() { return UINT32_MAX; }  

    PCG(uint64_t seed, uint64_t seq);

    uint32_t next();

    // Overload operator() to generate random numbers
    result_type operator()();
};
