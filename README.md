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

