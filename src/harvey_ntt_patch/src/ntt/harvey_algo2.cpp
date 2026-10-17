#include "ntt/ntt.hpp"

namespace ntt {
// Preconditions: x,y,w in [0,p), p<2^62, wprime=floor(w*2^64/p).
void harvey_algo2(u64& x, u64& y, u64 w, u64 wprime, u64 p) {
    u64 xp = x + y;
    if (xp >= p) xp -= p;
    const u64 t = x >= y ? x - y : x + p - y;
    const u64 q = static_cast<u64>((u128(wprime) * t) >> 64);
    u64 yp = w * t - q * p; // unsigned wraparound intentional
    if (yp >= p) yp -= p;
    x = xp;
    y = yp;
}
} // namespace ntt
