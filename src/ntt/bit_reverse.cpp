#include "convolution/ntt/ntt.hpp"
#include <algorithm>
#include <stdexcept>

namespace ntt {
u64 reverse_index(u64 index, unsigned bits) {
    if (bits > 64) throw std::invalid_argument("bits must be <= 64");
    if (bits == 0) return 0;
    return __builtin_bitreverse64(index) >> (64 - bits);
}

void bit_reverse(std::vector<u64>& a) {
    const std::size_t n = a.size();
    if (n == 0 || (n & (n - 1)) != 0)
        throw std::invalid_argument("NTT length must be a nonzero power of two");
    unsigned bits = 0;
    for (std::size_t t = n; t > 1; t >>= 1) ++bits;
    for (std::size_t i = 0; i < n; ++i) {
        const auto j = static_cast<std::size_t>(reverse_index(i, bits));
        if (i < j) std::swap(a[i], a[j]);
    }
}
} // namespace ntt
