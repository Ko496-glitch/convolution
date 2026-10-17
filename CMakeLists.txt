cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER "clang++")

project(ToeplitzConvolution VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
        -O3
        -march=armv8-a+simd
        -fvectorize
        -fslp-vectorize
        -ffast-math
        -Wall
        -Wextra
    )
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(FFTW3 REQUIRED IMPORTED_TARGET fftw3)

find_library(NTL_LIB ntl REQUIRED PATHS /opt/homebrew/opt/ntl/lib)
find_library(GMP_LIB gmp REQUIRED PATHS /opt/homebrew/opt/gmp/lib)
set(LINBOX_ROOT "$ENV{HOME}/local/linbox")

include_directories(${LINBOX_ROOT}/include)
link_directories(${LINBOX_ROOT}/lib)
add_subdirectory(src)

add_executable(toeplitz_bench
    benchmarks/bench_toeplitz.cpp
    utils/cpp_utils/print.cpp
)

target_include_directories(toeplitz_bench PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    /opt/homebrew/opt/ntl/include
    /opt/homebrew/opt/fftw/include
    $ENV{HOME}/local/linbox/include
)

target_link_libraries(toeplitz_bench
    PRIVATE
        convolution_core
        PkgConfig::FFTW3
        ${NTL_LIB}
        ${GMP_LIB}
        pthread
)

# Harvey Algorithm 2 benchmark
add_executable(harvey_algo2_bench
    benchmarks/bench_algo2.cpp
)

target_include_directories(harvey_algo2_bench PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    /opt/homebrew/opt/ntl/include
)

target_link_libraries(harvey_algo2_bench PRIVATE
    ${NTL_LIB}
    ${GMP_LIB}
)

add_executable(harvey_fft_bench
    benchmarks/bench_harvey_ntt.cpp
)

target_link_libraries(harvey_fft_bench
    PRIVATE
        ${NTL_LIB}
        ${GMP_LIB}
)

target_include_directories(harvey_fft_bench PRIVATE
    /opt/homebrew/opt/ntl/include
)

add_library(harvey_ntt
    src/butterfly/harvey_algo2.cpp
    src/ntt/ntt.cpp
    src/ntt/bit_reverse.cpp
    src/ntt/twiddle_table.cpp
)

target_include_directories(harvey_ntt PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(test_harvey_ntt
    tests/test_harvey_ntt.cpp
)

target_link_libraries(test_harvey_ntt PRIVATE
    harvey_ntt
    ${NTL_LIB}
    ${GMP_LIB}
)

enable_testing()
add_test(NAME harvey_ntt_correctness COMMAND test_harvey_ntt)


target_link_libraries(harvey_algo2_bench PRIVATE
    harvey_ntt
    ${NTL_LIB}
    ${GMP_LIB}
)