
#include <cstdint>

namespace ntt {

using u64 = std::uint64_t;
using u128 = unsigned __int128;

// Harvey Algorithm 2: NTL-style butterfly.
//
// Preconditions:
// 0 <= x, y, w < p
// p < 2^62
// wprime = floor(w * 2^64 / p)

inline void harvey_algo2(
    u64& x,
    u64& y,
    u64 w,
    u64 wprime,
    u64 p
) {
    // X' = (X + Y) mod p
    u64 xp = x + y;

    if (xp >= p)
        xp -= p;

    // T = (X - Y) mod p
    u64 t = (x >= y)
        ? x - y
        : x + p - y;

    // Q = floor(W' * T / 2^64)
    u64 q = static_cast<u64>(
        (u128(wprime) * t) >> 64
    );

    // R = W*T - Q*p
    // Unsigned 64-bit wraparound is intentional.
    u64 yp = w * t - q * p;

    // Reduce R from [0, 2p) to [0, p).
    if (yp >= p)
        yp -= p;

    x = xp;
    y = yp;
}

} // namespace ntt
