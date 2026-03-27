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

---

## ⚙️ C++ Development Guidelines (LLVM/NTL)

| Feature | Implementation | Goal |
| :--- | :--- | :--- |
| **Static Polymorphism** | Use **CRTP** over `virtual` functions | Ensure compile-time inlining & zero vtable overhead |
| **Member Ordering** | Order by size (**Largest → Smallest**) | Eliminate internal memory padding |
| **Cache Alignment** | Apply `alignas(64)` to core structures | Optimal cache-line alignment & fetch speed |
| **Empty Base Opt.** | Use `[[no_unique_address]]` | Zero-byte overhead for stateless classes |
| **Pointer Aliasing** | Mark math kernels with `__restrict__` | Enable aggressive SIMD auto-vectorization |
| **Modern Modules** | Use **.ixx / .cppm** (C++20) | Stop header bloat and NTL macro leakage |
| **Template Logic** | Use `std::concepts` / `requires` | Clearer constraints over SFINAE |
| **Global State** | Use `inline constexpr` (no `extern`) | Avoid Static Initialization Order Fiasco |
| **Hot Path Alloc** | Pass pre-allocated workspace buffers | Zero `malloc`/`new` in benchmark loops |
| **Exception Spec** | Mark kernels `noexcept` | Skip unnecessary stack-unwinding logic |
| **Branch Hints** | Use `[[likely]]` / `[[unlikely]]` | Guide the CPU branch predictor |
| **Complexity** | **Rule of Seven**: Max 7–10 members | Decompose large classes into smaller structs |
| **Const-Correctness** | Pass NTL types by `const &` | Prevent expensive deep copies of fields/vectors |

---

## 🦀 Rust Development Guidelines (Nalgebra)

| Feature | Implementation | Goal |
| :--- | :--- | :--- |
| **Static Dispatch** | **Traits with Generics** (no `dyn`) | Compile-time monomorphization |
| **Data Layout** | Use `#[repr(C)]` (Large → Small) | Predictable layout & eliminated padding |
| **Zero-Sized Types** | Use ZSTs for stateless logic | Native zero-byte optimization |
| **Nalgebra Storage** | Leverage `Storage` traits | Support both stack and heap-allocated data |
| **Unsafe Protocol** | Required `// SAFETY:` comments | Explicit documentation of invariants |
| **Performance** | Use **Raw Pointers** (`*const T`) | Bypass bounds checks in critical loops |
| **Allocations** | Pass mutable slices `&mut [T]` | Prevent heap churn during timing |
| **Inlining** | Apply `#[inline(always)]` | Assist LLVM in merging math operations |
| **Error Handling** | Use `panic!` for internal invariants | Match C++ `assert` performance profile |
| **Vectorization** | Utilize `std::simd` | Explicit SIMD where auto-vec falls short |

---

## 🛠 Toolchain & Compilation

### 1. C++ Toolchain (LLVM/Clang)
* **Compiler:** Latest **Clang** for cutting-edge C++20 features and optimization passes.
* **Workflow:** Mandatory `clang-format` and `clang-tidy` (performance profile) after every commit.
* **Analysis:** Built to support **Clang Front-end tools** (AST matchers) for automated research audits.
* **Optimization Flags:** `-O3 -march=native -ffast-math` to capture peak CPU throughput.

### 2. Rust Toolchain (rustc)
* **Compiler:** Latest stable `rustc` via `cargo`.
* **Linter:** `cargo clippy` to identify non-idiomatic code or performance regressions.
* **Optimization:** Benchmarks must run with `--release` to enable the full LLVM pipeline.
