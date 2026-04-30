#pragma once
#include <NTL/vec_ZZ_p.h>
#include <vector>

template <typename T> void generate_toeplitz_data(T &vec, long n) {
  vec.SetLength(2 * n - 1); // For NTL
  for (long i = 0; i < 2 * n - 1; ++i) {
    random(vec[i]);
  }
}

extern NTL::vec_ZZ_p generate_toeplitz_vector(long n);

NTL::vec_ZZ_p mul_norm_(const NTL::vec_ZZ_p &input,
                        const NTL::vec_ZZ_p &tope_mat, long n);

void fftw_c2c(const double *input, const double *kernel, double *output, int n);
void fftw_real(const double *input, const double *kernel, double *output,
               int n);

void ntl_fft(const NTL::vec_ZZ_p &input, const NTL::vec_ZZ_p &toep,
             NTL::vec_ZZ_p &output);

void compare_precision(const std::vector<double>& fftw_out,
                       const NTL::vec_ZZ_p& ntl_out,
                       long mod);
long mod_normalize(long x, long mod);