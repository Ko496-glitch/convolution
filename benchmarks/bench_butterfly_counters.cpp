
#include "convolution/butterfly/harvey.hpp"
#include "counters/bench.h"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <iomanip>
using ntt::u64;
using ntt::u128;

struct ButterflyInput {
    u64 x, y, w, wprime;
};


int main() {
    constexpr std::size_t N = 4096;
    constexpr u64 p = 2013265921ULL;

    if (!counters::has_performance_counters()) {
        std::cerr << "Hardware CPU counters unavailable\n";
        return EXIT_FAILURE;
    }

    std::vector<ButterflyInput> initial(N);

    for (std::size_t i = 0; i < N; ++i) {
        const u64 w = (17 * i + 1) % p;
        initial[i] = {
            (123 * i + 7) % p,
            (321 * i + 11) % p,
            w,
            static_cast<u64>((u128(w) << 64) / p)
        };
    }

    // Independent buffers, identical starting values.
    auto harvey_inputs = initial;
    auto barrett_inputs = initial;

    // Precompute outside the timed region.
    const u128 mu = ntt::barrett_reciprocal(p);

    auto run_harvey = [&] {
        for (auto& a : harvey_inputs) {
            ntt::harvey_algo2(
                a.x, a.y, a.w, a.wprime, p
            );
        }
    };

    auto run_barrett = [&] {
        for (auto& a : barrett_inputs) {
            ntt::harvey_algo2_barrett(
                a.x, a.y, a.w, mu, p
            );
        }
    };

    auto harvey_result = counters::bench(run_harvey);
    auto barrett_result = counters::bench(run_barrett);

    std::cout << std::fixed << std::setprecision(4);

    std::cout << "\nHarvey Algorithm 2\n";
    std::cout << "Cycles/butterfly: "
              << harvey_result.cycles() / N << '\n';
    std::cout << "Instructions/butterfly: "
              << harvey_result.instructions() / N << '\n';
    std::cout << "ns/butterfly: "
              << harvey_result.elapsed_ns() / N << '\n';

    std::cout << "\nHarvey Algorithm 2 + Barrett\n";
    std::cout << "Cycles/butterfly: "
              << barrett_result.cycles() / N << '\n';
    std::cout << "Instructions/butterfly: "
              << barrett_result.instructions() / N << '\n';
    std::cout << "ns/butterfly: "
              << barrett_result.elapsed_ns() / N << '\n';

    // Compare final outputs only if the two benchmarks
    // executed the same number of repetitions.
    const bool same_iterations =
        harvey_result.iteration_count() ==
        barrett_result.iteration_count();

    bool same_outputs = same_iterations;

    if (same_iterations) {
        for (std::size_t i = 0; i < N; ++i) {
            if (harvey_inputs[i].x != barrett_inputs[i].x ||
                harvey_inputs[i].y != barrett_inputs[i].y) {
                same_outputs = false;
                break;
            }
        }
    }

    std::cout << "\nSame benchmark repetitions: "
              << (same_iterations ? "YES" : "NO") << '\n';
    std::cout << "Final outputs match: "
              << (same_outputs ? "YES" : "NO") << '\n';

    return EXIT_SUCCESS;
}
