#include "convolution/ntt/ntt.hpp"
#include <stdexcept>
#include <utility>

namespace ntt {
u64 mod_pow(u64 base, u64 exp, u64 p) {
    if (p < 2) throw std::invalid_argument("modulus must be >= 2");
    base %= p;
    u64 result = 1;
    while (exp > 0) {
        if (exp & 1) result = static_cast<u64>(u128(result) * base % p);
        base = static_cast<u64>(u128(base) * base % p);
        exp >>= 1;
    }
    return result;
}

TwiddleTable make_twiddle_table(u64 n, u64 p, u64 generator) {
    if (n == 0 || (n & (n - 1)) != 0)
        throw std::invalid_argument("NTT size must be a nonzero power of two");
    if (p < 3 || p >= (u64{1} << 62) || (p - 1) % n != 0)
        throw std::invalid_argument("Invalid NTT modulus or length");
    if (generator == 0 || generator >= p)
        throw std::invalid_argument("Invalid primitive root");
    // Caller must guarantee p is prime and generator is primitive modulo p.
    TwiddleTable table;
    for (u64 len = 2; len <= n; len *= 2) {
        const u64 omega = mod_pow(generator, (p - 1) / len, p);
        std::vector<Twiddle> stage;
        u64 w = 1;
        for (u64 j = 0; j < len / 2; ++j) {
            const u64 wprime = static_cast<u64>((u128(w) << 64) / p);
            stage.emplace_back(w, wprime);
            w = static_cast<u64>(u128(w) * omega % p);
        }
        table.push_back(std::move(stage));
    }
    return table;
}
} // namespace ntt
