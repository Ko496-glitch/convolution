<div align="center">
  <h1>🌀 Convolution & Toeplitz Research</h1>
  <p><b>High-Performance Linear Algebra & FFT implementations in C++20 and Rust</b></p>

  <p>
    <img src="https://img.shields.io/badge/C%2B%2B-20-blue.svg?style=flat-square&logo=c%2B%2B" />
    <img src="https://img.shields.io/badge/Rust-Stable-orange.svg?style=flat-square&logo=rust" />
    <img src="https://img.shields.io/badge/Compiler-Clang%20LLVM-green.svg?style=flat-square" />
    <img src="https://img.shields.io/badge/Research-Drexel%20University-yellow.svg?style=flat-square" />
  </p>
</div>

---

### 📖 Project Description
This project implements high-performance operations with **Toeplitz matrices**, specifically focusing on matrix-vector products and the solution of Toeplitz systems. A core component of this research is the exploration of **Fast Fourier Transform (FFT)** algorithms as the primary mechanism for reducing computational complexity to $O(n \log n)$. 

All implementations are benchmarked against state-of-the-art baselines to evaluate language-specific optimization capabilities in C++ and Rust.

### 👥 Supervision & Contact
This research is conducted under the supervision of **Professor Jeremy Johnson** and **Kartik Ohlan**.

* **Lead Researcher:** Kartik Ohlan
* **Email:** [Kartik7ohlan@gmail.com](mailto:Kartik7ohlan@gmail.com)

---

### 📂 File Structure
```text
convolution/
├── Makefile
├── README.md
├── data/
├── src/
│   ├── cpp_impl/
│   │   ├── toeplitz.hpp
│   │   └── toeplitz.cpp
│   └── rust_impl/
│       ├── Cargo.toml
│       └── src/
│           ├── lib.rs
│           └── main.rs
├── utils/
│   ├── cpp_utils/
│   │   ├── bench_harness.cpp
│   │   └── io_helpers.hpp
│   └── rust_utils/
│       └── mod.rs
----------------------------------------------------C++ Guidelines----------------------------------------------------------------------------------------

Static Polymorphism: Use CRTP instead of virtual functions to ensure compile-time inlining.

Member Ordering: Order members by size (largest to smallest) to eliminate internal padding.

Cache Alignment: Apply alignas(64) to core structures for optimal cache-line performance.

Empty Base Optimization: Use [[no_unique_address]] for stateless classes to ensure zero-byte overhead.

Pointer Aliasing: Mark math kernel pointers with **restrict** to enable SIMD auto-vectorization.

C++20 Modules: Use .ixx / .cppm to stop header bloat and NTL macro leakage.

Concepts: Use std::concepts for template constraints instead of SFINAE.

No Externs: Use inline constexpr for globals to avoid static initialization order issues.

No Hidden Allocations: Pass pre-allocated workspace buffers to avoid malloc in the hot path.

Noexcept: Mark all math kernels noexcept to skip unnecessary stack-unwinding logic.

Branch Hints: Use [[likely]] / [[unlikely]] to guide the CPU branch predictor.

Rule of Seven: Break down any class with more than 7–10 members into smaller structs.

Const-Correctness: Pass NTL types by const & to prevent expensive deep copies.

All the major computation API's will be used from NTL


---------------------------------------------------------------Rust Guidelines ---------------------------------------------------------------------------

Static Dispatch: Use Traits with Generics instead of dyn to ensure compile-time monomorphization.

Data Layout: Use #[repr(C)] and order fields largest to smallest to eliminate padding.

Zero-Sized Types: Use ZSTs for stateless logic; Rust optimizes these to zero bytes automatically.

Nalgebra Storage: Use Storage traits so kernels work on both stack and heap data.

Unsafe Safety: Every unsafe block must have a // SAFETY: comment explaining the invariants.

Raw Pointers: Use *const T in hot loops to bypass bounds checks for maximum performance.

No Allocations: Pass mutable slices &mut [T] to avoid heap churn during benchmarks.

Inlining: Apply #[inline(always)] to small math kernels to help LLVM merge operations.

Error Handling: Use panic! for logic invariants to match C++ assert behavior.

SIMD Intrinsics: Use std::simd where nalgebra auto-vectorization falls short.


---------------------------------------------------------------Compiler Guidelines ----------------------------------------------------------------------

1) C++ Toolchain (LLVM/Clang)
  
Compiler: Use the latest Clang version to leverage cutting-edge C++20 features and optimization passes.

Formatting: Run clang-format after every commit to maintain a consistent style across the cpp_impl and utils.

Static Analysis: Use clang-tidy for performance linting, specifically checking for unnecessary copies and memory alignment.

Custom Tooling: Architecture must support Clang Front-end tools for automated research-specific audits (e.g., custom AST matchers for matrix logic).

Optimization Flags: Build with -O3 -march=native -ffast-math to ensure the benchmark captures the CPU's maximum throughput.


2) Rust Toolchain (rustC)

Compiler: Use the latest stable rustc via cargo for all builds.

Linter: Use cargo clippy to catch non-idiomatic code or performance regressions in the rust_impl.

Optimization: Always benchmark using --release mode to enable LLVM’s full optimization pipeline.
