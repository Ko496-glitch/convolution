
#include "convolution/butterfly/harvey.hpp"
#include <NTL/ZZ.h>
#include <NTL/Tools.h>

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

using u64 = std::uint64_t;
using u128 = unsigned __int128;

struct Butterfly {
    u64 x, y, w, wprime;
};

u64 next_random(u64& state) {
    state ^= state >> 12;
    state ^= state << 25;
    state ^= state >> 27;
    return state * 2685821657736338717ULL;
}

int main(int argc, char** argv) {
    constexpr u64 p = 2013265921ULL;
    constexpr std::size_t count = 4096;

    const std::size_t repeats =
        argc > 1 ? std::stoull(argv[1]) : 3000;

    if (repeats == 0)
        throw std::invalid_argument(
            "Repetitions must be positive"
        );

    std::vector<Butterfly> data(count);
    u64 seed = 123456789;

    // Precompute everything outside the timer.
    for (auto& b : data) {
        b.x = next_random(seed) % p;
        b.y = next_random(seed) % p;
        b.w = 1 + next_random(seed) % (p - 1);

        b.wprime = static_cast<u64>(
            (u128(b.w) << 64) / p
        );
    }

    // Check each butterfly against NTL.
    NTL::ZZ P = NTL::conv<NTL::ZZ>(
        static_cast<long>(p)
    );

    for (const auto& b : data) {
        u64 x = b.x, y = b.y;

        ntt::harvey_algo2(x, y, b.w, b.wprime, p);

        NTL::ZZ X = NTL::conv<NTL::ZZ>(
            static_cast<long>(b.x)
        );
        NTL::ZZ Y = NTL::conv<NTL::ZZ>(
            static_cast<long>(b.y)
        );
        NTL::ZZ W = NTL::conv<NTL::ZZ>(
            static_cast<long>(b.w)
        );

        NTL::ZZ expected_x = (X + Y) % P;
        NTL::ZZ expected_y = (W * (X - Y)) % P;

        if (expected_y < 0)
            expected_y += P;

        if (x != static_cast<u64>(
                     NTL::conv<long>(expected_x)) ||
            y != static_cast<u64>(
                     NTL::conv<long>(expected_y))) {
            std::cerr << "Correctness FAILED\n";
            return 1;
        }
    }

    std::cout << "PASS: " << count
              << " NTL correctness checks\n";

    auto run = [&]() {
        for (std::size_t r = 0; r < repeats; ++r) {
            for (auto& b : data) {
                ntt::harvey_algo2(
                    b.x, b.y, b.w, b.wprime, p
                );
            }
        }
    };

    // Warm-up
    run();

    // std::chrono measurement
    auto start = std::chrono::steady_clock::now();
    run();
    auto end = std::chrono::steady_clock::now();

    double chrono_seconds =
        std::chrono::duration<double>(end - start).count();

    // NTL timing
    double ntl_start = NTL::GetTime();
    run();
    double ntl_seconds = NTL::GetTime() - ntl_start;

    u64 checksum = 0;
    for (const auto& b : data)
        checksum ^= b.x + b.y;

    const double total =
        static_cast<double>(count) * repeats;

    std::cout << std::fixed << std::setprecision(4);

    std::cout << "Butterflies: " << total << '\n';

    std::cout << "chrono: " << chrono_seconds
              << " seconds\n";

    std::cout << "chrono: "
              << chrono_seconds * 1e9 / total
              << " ns/butterfly\n";

    std::cout << "NTL GetTime: " << ntl_seconds
              << " seconds\n";

    std::cout << "NTL GetTime: "
              << ntl_seconds * 1e9 / total
              << " ns/butterfly\n";

    std::cout << "Checksum: " << checksum << '\n';
}
