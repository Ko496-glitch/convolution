#include <iostream>
#include <utility>
#include <vector>

#include <random> 

#include <NTL/tools.h>
#include <NTL/vec_ZZ_p.h>
#include <fftw3.h> 

using namespace NTL;

void generate_toeplitz_data(std::vector<double> &vec, long n) {
  vec.resize(2 * n - 1);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);
  for (long i = 0; i < 2 * n - 1; ++i)
    vec[i] = dis(gen);
}

vec_ZZ_p mul_norm_(const vec_ZZ_p &input, const vec_ZZ_p &tope_mat, long n) {
  vec_ZZ_p res;
  res.SetLength(n);
  for (long i = 0; i < n; ++i) {
    ZZ_p sum{0};
    for (long j = 0; j < n; ++j) {
      long v = (n - 1) + (j - i);
      ZZ_p prod;
      mul(prod, tope_mat[v], input[j]);
      add(sum, sum, prod);
    }
    res[i] = sum;
  }
  return res;
}

void fftw_toeplitz_convolve(const double *input, const double *mat,
                            double *output, int n) {
  int m = 1;
  while (m < 2 * n)
    m <<= 1; // FIX

  fftw_complex *in_fft = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m);
  fftw_complex *mat_fft = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m);
  fftw_complex *output_fft =
      (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m);

  for (int i = 0; i < m; ++i) {
    in_fft[i][0] = (i < n) ? input[i] : 0.0; // FIX
    in_fft[i][1] = 0.0;

    mat_fft[i][0] = 0.0;
    mat_fft[i][1] = 0.0;
  }

  for (int i = 0; i < n; ++i) {
    mat_fft[i][0] = mat[(n - 1) - i];
  }
  for (int i = 0; i < n; ++i) {
    mat_fft[m - n + i][0] = mat[(n - 1) + i]; // FIX
  }

  fftw_plan p_in =
      fftw_plan_dft_1d(m, in_fft, in_fft, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_plan p_mat =
      fftw_plan_dft_1d(m, mat_fft, mat_fft, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_plan p_inv =
      fftw_plan_dft_1d(m, output_fft, output_fft, FFTW_BACKWARD, FFTW_ESTIMATE);

  fftw_execute(p_in);  // FIX
  fftw_execute(p_mat); // FIX

  for (int i = 0; i < m; ++i) {
    output_fft[i][0] =
        in_fft[i][0] * mat_fft[i][0] - in_fft[i][1] * mat_fft[i][1]; // FIX
    output_fft[i][1] =
        in_fft[i][0] * mat_fft[i][1] + in_fft[i][1] * mat_fft[i][0]; // FIX
  }

  fftw_execute(p_inv); // FIX

  for (int i = 0; i < n; ++i) { // FIX: only first n outputs
    output[i] = output_fft[i][0] / m;
  }

  fftw_destroy_plan(p_in);
  fftw_destroy_plan(p_mat);
  fftw_destroy_plan(p_inv);
  fftw_free(in_fft);
  fftw_free(mat_fft);
  fftw_free(output_fft);
}