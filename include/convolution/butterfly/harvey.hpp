#pragma once

#include <cstdint>

namespace ntt {

using u64 = std::uint64_t;
using u128 = unsigned __int128;

void harvey_algo2(
    u64& x,
    u64& y,
    u64 w,
    u64 wprime,
    u64 p
);

u128 barrett_reciprocal(u64 p);

void harvey_algo2_barrett(
    u64& x,
    u64& y,
    u64 w,
    u128 mu,
    u64 p
);
}
