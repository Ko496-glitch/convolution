// SETUP: Outside the timer
uint64_t p = 17;
uint64_t w = 9;

uint64_t wprime = static_cast<uint64_t>(
    (static_cast<unsigned __int128>(w) << 64) / p
);

// Benchmark inputs
uint64_t x = 5;
uint64_t y = 3;

// START TIMER
auto start = std::chrono::steady_clock::now();

for (size_t i = 0; i < iterations; ++i) {
    harvey_algo2(x, y, w, wprime, p);
}

auto end = std::chrono::steady_clock::now();
// END TIMER