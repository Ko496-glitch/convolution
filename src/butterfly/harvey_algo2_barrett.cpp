
#include "convolution/butterfly/harvey.hpp"

#include <cassert>

namespace ntt {

namespace {

u128 mul_high128(u128 a, u128 b) {
    const u64 a0 = static_cast<u64>(a);
    const u64 a1 = static_cast<u64>(a >> 64);
    const u64 b0 = static_cast<u64>(b);
    const u64 b1 = static_cast<u64>(b >> 64);

    const u128 p00 = u128(a0) * b0;
    const u128 p01 = u128(a0) * b1;
    const u128 p10 = u128(a1) * b0;
    const u128 p11 = u128(a1) * b1;

    const u128 middle =
        (p00 >> 64) + u64(p01) + u64(p10);

    return p11 + (p01 >> 64) + (p10 >> 64)
               + (middle >> 64);
}

} // namespace

u128 barrett_reciprocal(u64 p) {
    assert(p >= 3 && (p & 1));
    return ~u128(0) / p;
}

void harvey_algo2_barrett(
    u64& x, u64& y, u64 w, u128 mu, u64 p
) {
    assert(p >= 3 && p < (u64(1) << 62));
    assert(x < p && y < p && w < p);

    u64 xp = x + y;
    if (xp >= p)
        xp -= p;

    const u64 t = x >= y ? x - y : x + p - y;

    const u128 z = u128(w) * t;

    
    const u128 q = mul_high128(z, mu);


    u128 r = z - q * p;

    if (r >= p)
        r -= p;
    if (r >= p)
        r -= p;

    x = xp;
    y = static_cast<u64>(r);
}

} // namespace ntt
