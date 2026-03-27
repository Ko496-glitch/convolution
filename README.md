
This project will implement high performance operations with Toeplitz matrices including matrix vector product and the solution of Toeplitz systems.  The project will also explore fast Fourier transform algorithms as they are a key tool in computing with Toeplitz matrices.  Implementations produced will be benchmarked against the state-of-the-art.

This research is conducted under the supervision of Professor Jeremy Johnson and Kartik Ohlan.

For inquiries, contributions, or technical discussions regarding the Toeplitz matrix implementations or benchmarking methodology, please contact:

Lead Researcher: Kartik Ohlan

Email: Kartik7ohlan@gmail.com


File Structure for this repo is :

/convolution
├── Makefile  
├── README.md  
├── /src
│ ├── /cpp_impl
│ │ ├── toeplitz.hpp  
│ │ └── toeplitz.cpp  
│ └── /rust_impl  
│ ├── Cargo.toml  
│ └── src
│ ├── lib.rs  
│ └── main.rs  
├── /utils
│ ├── /cpp_utils
│ │ ├── bench_harness.cpp
│ │ └── io_helpers.hpp  
│ └── /rust_utils
│ └── mod.rs  
└── /data

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
