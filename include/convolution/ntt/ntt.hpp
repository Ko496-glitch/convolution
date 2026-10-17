#pragma once
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

namespace ntt {
using u64 = std::uint64_t;
using u128 = unsigned __int128;
using Twiddle = std::pair<u64, u64>; // {W, W'}
using TwiddleTable = std::vector<std::vector<Twiddle>>;

u64 reverse_index(u64 index, unsigned bits);
void bit_reverse(std::vector<u64>& a);
u64 mod_pow(u64 base, u64 exp, u64 p);
TwiddleTable make_twiddle_table(u64 n, u64 p, u64 generator);
void forward_ntt(std::vector<u64>& a, u64 p, const TwiddleTable& table);
} // namespace ntt
