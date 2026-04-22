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

add_subdirectory(src)

add_executable(toeplitz_bench
    utils/cpp_utils/routine.cpp
    utils/cpp_utils/print.cpp
)

target_include_directories(toeplitz_bench PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    /opt/homebrew/opt/ntl/include
    /opt/homebrew/opt/fftw/include
)

target_link_libraries(toeplitz_bench
    PRIVATE
        convolution_core
        PkgConfig::FFTW3
        ${NTL_LIB}
        ${GMP_LIB}
        pthread
)