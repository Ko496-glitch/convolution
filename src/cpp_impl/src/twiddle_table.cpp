
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <utility>
#include <vector>

namespace ntt {

using u64 = std::uint64_t;
using u128 = unsigned __int128;

// Each pair stores {W, W'}.
using Twiddle = std::pair<u64, u64>;

// Computes base^exp mod p.
u64 mod_pow(u64 base, u64 exp, u64 p) {
    u64 result = 1;

    while (exp > 0) {
        if (exp & 1)
            result = static_cast<u64>(
                u128(result) * base % p
            );

        base = static_cast<u64>(
            u128(base) * base % p
        );

        exp >>= 1;
    }

    return result;
}

// Build a twiddle table for every radix-2 stage.
// Preconditions:
// - p is prime.
// - generator is a primitive root modulo p.
// - n is a power of 2 dividing p - 1.
std::vector<std::vector<Twiddle>>
make_twiddle_table(u64 n, u64 p, u64 generator) {
    if (n == 0 || (n & (n - 1)) != 0)
        throw std::invalid_argument(
            "NTT size must be a power of two"
        );

    if (p < 3 || p >= (u64{1} << 62) ||
        (p - 1) % n != 0)
        throw std::invalid_argument(
            "Invalid NTT modulus or length"
        );

    if (generator == 0 || generator >= p)
        throw std::invalid_argument(
            "Invalid primitive root"
        );

    std::vector<std::vector<Twiddle>> table;

    // Stages: len = 2, 4, 8, ..., n
    for (u64 len = 2; len <= n; len *= 2) {
        // Primitive len-th root of unity.
        u64 omega = mod_pow(
            generator, (p - 1) / len, p
        );

        std::vector<Twiddle> stage;
        u64 w = 1;

        for (u64 j = 0; j < len / 2; ++j) {
            // W' = floor(W * 2^64 / p)
            u64 wprime = static_cast<u64>(
                (u128(w) << 64) / p
            );

            stage.emplace_back(w, wprime);

            // Next power: W = W * omega mod p
            w = static_cast<u64>(
                u128(w) * omega % p
            );
        }

        table.push_back(std::move(stage));
    }

    return table;
}

} // namespace ntt
