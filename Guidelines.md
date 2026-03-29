## ⚡ Guidelines (C++ / RUST)

### 🛠️ MEMORY & DATA LAYOUT
* **Packing:** Order members by size (Largest → Smallest) to kill internal padding.
* **Alignment:** Use `alignas(64)` / `#[repr(C)]` for cache-line optimization.
* **Zero-Overhead:** Use `[[no_unique_address]]` (C++) or ZSTs (Rust) for stateless logic.
* **No Allocations:** Pass pre-allocated buffers/slices; avoid `malloc` or `Vec` in hot paths.

### 🏎️ KERNEL OPTIMIZATION
* **Dispatch:** Prefer **Static** (CRTP in C++, Generics in Rust) over `virtual`/`dyn`.
* **Vectorization:** Use `__restrict__` or raw pointers to enable SIMD auto-vectorization.
* **Hints:** Guide the CPU with `[[likely]]`/`[[unlikely]]` and `#[inline(always)]`.
* **Safety:** Mark kernels `noexcept` (C++) and use `// SAFETY` comments for `unsafe` (Rust).

### 🏗️ MODERN ARCHITECTURE
* **Standards:** Use **C++20 Concepts/Modules** and **Rust Traits** to stop header/macro bloat.
* **Const-Correctness:** Pass NTL/Nalgebra types by reference to skip expensive deep copies.
* **Rule of 7:** If a struct has >7 members, break it down. Smaller is faster.

### 🔧 THE TOOLCHAIN
| TASK         | LLVM / CLANG (C++)             | RUST / CARGO              |
|--------------|--------------------------------|---------------------------|
| **Compiler** | `clang++ -O3 -march=native`    | `cargo build --release`   |
| **Linting** | `clang-tidy` / `clang-format`  | `cargo clippy`            |
| **Special** | AST Matchers / `-ffast-math`   | `std::simd` intrinsics    |
