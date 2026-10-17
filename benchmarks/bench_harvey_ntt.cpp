#include <NTL/ZZ.h>
#include <NTL/Tools.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>

using u64 = std::uint64_t;
using u128 = unsigned __int128;

namespace {
constexpr std::size_t L = 2048;
constexpr u64 P = 2305843009213704193ULL; // 62-bit prime, P = 1 (mod 2048)
constexpr std::size_t BUTTERFLIES_PER_FFT = (L / 2) * 11;
struct Twiddle { u64 w, wp; };
using Table = std::vector<std::vector<Twiddle>>;

u64 mul_mod(u64 a, u64 b, u64 p) {
    return static_cast<u64>(u128(a) * b % p);
}
u64 pow_mod(u64 a, u64 e, u64 p) {
    u64 result = 1;
    for (; e != 0; e >>= 1) {
        if (e & 1) result = mul_mod(result, a, p);
        a = mul_mod(a, a, p);
    }
    return result;
}

// Harvey's paper, Algorithm 2: NTL-style Shoup butterfly.
// Requires X,Y in [0,p), W in (0,p), W' = floor(W*2^64/p), p < 2^63.
inline void algo2(u64& x, u64& y, const Twiddle& tw) {
    u64 sum = x + y;
    if (sum >= P) sum -= P;
    u64 t = (x >= y) ? (x - y) : (x + P - y);
    u64 q = static_cast<u64>((u128(tw.wp) * t) >> 64);
    u64 product = tw.w * t - q * P; // defined modulo 2^64
    if (product >= P) product -= P;
    x = sum;
    y = product;
}

u64 next_rand(u64& s) {
    s ^= s >> 12;
    s ^= s << 25;
    s ^= s >> 27;
    return s * 2685821657736338717ULL;
}

// Find primitive L-th root directly; no need for full primitive generator.
u64 find_root() {
    for (u64 candidate = 2; candidate < 100000; ++candidate) {
        u64 w = pow_mod(candidate, (P - 1) / L, P);
        if (pow_mod(w, L, P) == 1 && pow_mod(w, L / 2, P) != 1)
            return w;
    }
    throw std::runtime_error("No root found");
}

Table precompute(u64 root) {
    Table table(11);
    for (unsigned s = 0; s < 11; ++s) {
        std::size_t half = L >> (s + 1);
        u64 step = pow_mod(root, u64{1} << s, P);
        u64 w = 1;
        table[s].reserve(half);
        for (std::size_t k = 0; k < half; ++k) {
            table[s].push_back({w, static_cast<u64>((u128(w) << 64) / P)});
            w = mul_mod(w, step, P);
        }
    }
    return table;
}

// Same DIF schedule as Harvey's Algorithm 1. Output is bit-reversed.
void fft_butterflies(std::vector<u64>& data, const Table& table) {
    for (unsigned s = 0; s < 11; ++s) {
        std::size_t half = L >> (s + 1);
        std::size_t len = half * 2;
        for (std::size_t start = 0; start < L; start += len) {
            for (std::size_t k = 0; k < half; ++k) {
                algo2(data[start + k], data[start + k + half], table[s][k]);
            }
        }
    }
}

std::size_t reverse_index(std::size_t i) {
    return __builtin_bitreverse64(static_cast<u64>(i)) >> (64 - 11);
}

void verify_ntl(const std::vector<u64>& input,
                const std::vector<u64>& transformed,
                u64 root) {
    // Independent evaluation at 16 frequency indices with NTL::ZZ.
    NTL::ZZ modulus = NTL::conv<NTL::ZZ>(static_cast<long>(P));
    const std::size_t frequencies[] = {
        0,1,2,3,4,5,7,8,15,31,63,127,255,511,1023,2047
    };
    for (std::size_t frequency : frequencies) {
        u64 factor = pow_mod(root, frequency, P);
        u64 wk = 1;
        NTL::ZZ acc(0);
        for (u64 value : input) {
            NTL::ZZ V = NTL::conv<NTL::ZZ>(static_cast<long>(value));
            NTL::ZZ W = NTL::conv<NTL::ZZ>(static_cast<long>(wk));
            acc = (acc + V * W) % modulus;
            wk = mul_mod(wk, factor, P);
        }
        u64 expected = static_cast<u64>(NTL::conv<long>(acc));
        u64 got = transformed[reverse_index(frequency)];
        if (got != expected) {
            std::cerr << "NTL verification failed at frequency " << frequency
                      << ": got " << got << ", expected " << expected << '\n';
            std::exit(1);
        }
    }
    std::cout << "PASS: 16 frequency coefficients independently verified using NTL\n";
}

struct Trial { double chrono_s, ntl_s; };

double median(std::vector<double> x) {
    std::sort(x.begin(), x.end());
    const std::size_t m = x.size()/2;
    return x.size()%2 ? x[m] : (x[m-1]+x[m])/2;
}
}

int main(int argc, char** argv) {
    const std::size_t repeats = argc > 1 ? std::stoull(argv[1]) : 3000;
    const std::size_t trials = argc > 2 ? std::stoull(argv[2]) : 7;
    if (repeats == 0 || trials == 0) throw std::invalid_argument("repeats and trials must be nonzero");

    u64 seed = 0x123456789abcdefULL;
    std::vector<u64> initial(L);
    for (auto& v : initial) v = next_rand(seed) % P;
    u64 root = find_root();
    Table table = precompute(root);

    // Correctness is excluded from timing.
    auto checked = initial;
    fft_butterflies(checked, table);
    verify_ntl(initial, checked, root);

    // Warm up.
    auto warmup = initial;
    for (int k = 0; k < 20; ++k) fft_butterflies(warmup, table);

    std::vector<double> wall, cpu;
    u64 check = 0;
    for (std::size_t trial = 0; trial < trials; ++trial) {
        auto data = initial; // outside timer
        const double cpu_start = NTL::GetTime();
        const auto wall_start = std::chrono::steady_clock::now();
        for (std::size_t r = 0; r < repeats; ++r) fft_butterflies(data, table);
        const auto wall_end = std::chrono::steady_clock::now();
        const double cpu_end = NTL::GetTime();
        wall.push_back(std::chrono::duration<double>(wall_end - wall_start).count());
        cpu.push_back(cpu_end - cpu_start);
        // Read all final elements, preventing dead-code elimination.
        for (u64 value : data) check ^= value + 0x9e3779b97f4a7c15ULL;
    }
    const double operations = static_cast<double>(BUTTERFLIES_PER_FFT) * repeats;
    std::cout << std::fixed << std::setprecision(4)
              << "p: " << P << " (" << 62 << "-bit)\n"
              << "L: " << L << ", root: " << root << '\n'
              << "Butterflies/FFT: " << BUTTERFLIES_PER_FFT << '\n'
              << "Repetitions/trial: " << repeats << ", trials: " << trials << '\n'
              << "Butterflies/trial: " << static_cast<std::uint64_t>(operations) << '\n'
              << "Median chrono: " << median(wall)*1e9/operations << " ns/butterfly\n"
              << "Median NTL GetTime: " << median(cpu)*1e9/operations << " ns/butterfly\n"
              << "Checksum: " << check << '\n'
              << "Hardware cycles/butterfly: not measured (use a CPU performance counter)\n";
}
