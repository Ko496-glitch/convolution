#include "ntt/ntt.hpp"
#include <NTL/ZZ_p.h>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
    using ntt::u64;
    constexpr u64 p = 17, n = 8, generator = 3;
    std::vector<u64> input{0, 1, 2, 3, 4, 5, 6, 7};
    const auto table = ntt::make_twiddle_table(n, p, generator);
    std::vector<u64> actual = input;
    ntt::forward_ntt(actual, p, table);

    // Independent O(n^2) definition of the NTT using NTL field arithmetic.
    NTL::ZZ_p::init(NTL::to_ZZ(p));
    const NTL::ZZ_p omega = NTL::to_ZZ_p(static_cast<long>(ntt::mod_pow(generator, (p-1)/n, p)));
    for (u64 k = 0; k < n; ++k) {
        NTL::ZZ_p expected = NTL::to_ZZ_p(0);
        for (u64 j = 0; j < n; ++j) {
            expected += NTL::to_ZZ_p(static_cast<long>(input[j])) * NTL::power(omega, static_cast<long>(j*k));
        }
        const u64 expected_u64 = static_cast<u64>(NTL::conv<long>(NTL::rep(expected)));
        assert(actual[k] == expected_u64);
    }
    std::cout << "PASS: 8-point NTT matches direct NTL DFT modulo 17\n";
    for (u64 x : actual) std::cout << x << ' ';
    std::cout << '\n';
}
