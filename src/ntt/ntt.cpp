#include "convolution/ntt/ntt.hpp"
#include "convolution/butterfly/harvey.hpp"
#include <stdexcept>

namespace ntt {
// Forward radix-2 decimation-in-frequency NTT, unnormalized.
// table must come from make_twiddle_table(a.size(), p, generator).
void forward_ntt(std::vector<u64>& a, u64 p, const TwiddleTable& table) {
    const std::size_t n = a.size();
    if (n == 0 || (n & (n - 1)) != 0)
        throw std::invalid_argument("NTT length must be a nonzero power of two");
    if (p < 3 || p >= (u64{1} << 62))
        throw std::invalid_argument("Unsupported modulus");
    unsigned stages = 0;
    for (std::size_t t = n; t > 1; t >>= 1) ++stages;
    if (table.size() != stages)
        throw std::invalid_argument("Twiddle table has wrong number of stages");
    for (auto value : a)
        if (value >= p) throw std::invalid_argument("Input must be reduced modulo p");

    // Harvey's butterfly is decimation-in-frequency (DIF):
    // first combine large blocks, then smaller blocks.
    for (unsigned remaining = stages; remaining > 0; --remaining) {
        const unsigned stage_index = remaining - 1;
        const std::size_t half = std::size_t{1} << stage_index;
        const std::size_t len = half * 2;
        if (table[stage_index].size() != half)
            throw std::invalid_argument("Twiddle table has wrong stage length");
        for (std::size_t start = 0; start < n; start += len) {
            for (std::size_t j = 0; j < half; ++j) {
                const auto [w, wprime] = table[stage_index][j];
                harvey_algo2(a[start + j], a[start + j + half],
                             w, wprime, p);
            }
        }
    }
    // DIF produces bit-reversed output ordering.
    bit_reverse(a);
}
} // namespace ntt
