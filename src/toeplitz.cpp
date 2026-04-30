#include <iostream>
#include <utility>
#include <vector>

#include <random>

#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>
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

void fftw_c2c(const double *input, const double *mat, double *output, int n) {
  int m = 1;
  while (m < 2 * n)
    m <<= 1;

  fftw_complex *in_fft = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m);
  fftw_complex *mat_fft = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m);
  fftw_complex *output_fft =
      (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m);

  for (int i = 0; i < m; ++i) {
    in_fft[i][0] = (i < n) ? input[i] : 0.0;
    in_fft[i][1] = 0.0;

    mat_fft[i][0] = 0.0;
    mat_fft[i][1] = 0.0;
  }

  for (int i = 0; i < n; ++i) {
    mat_fft[i][0] = mat[(n - 1) - i];
  }
  for (int i = 0; i < n; ++i) {
    mat_fft[m - n + i][0] = mat[(n - 1) + i];
  }

  fftw_plan p_in =
      fftw_plan_dft_1d(m, in_fft, in_fft, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_plan p_mat =
      fftw_plan_dft_1d(m, mat_fft, mat_fft, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_plan p_inv =
      fftw_plan_dft_1d(m, output_fft, output_fft, FFTW_BACKWARD, FFTW_ESTIMATE);

  fftw_execute(p_in);
  fftw_execute(p_mat);

  for (int i = 0; i < m; ++i) {
    output_fft[i][0] =
        in_fft[i][0] * mat_fft[i][0] - in_fft[i][1] * mat_fft[i][1];
    output_fft[i][1] =
        in_fft[i][0] * mat_fft[i][1] + in_fft[i][1] * mat_fft[i][0];
  }

  fftw_execute(p_inv);

  for (int i = 0; i < n; ++i) {
    output[i] = output_fft[i][0] / m;
  }

  fftw_destroy_plan(p_in);
  fftw_destroy_plan(p_mat);
  fftw_destroy_plan(p_inv);
  fftw_free(in_fft);
  fftw_free(mat_fft);
  fftw_free(output_fft);
}

void fftw_real(const double *input, const double *mat, double *output, int n) {
  int m = 1;
  while (m < 2 * n)
    m <<= 1;

  double *in_fft = (double *)fftw_malloc(sizeof(double) * m);
  double *mat_fft = (double *)fftw_malloc(sizeof(double) * m);
  double *out_fft = (double *)fftw_malloc(sizeof(double) * m);

  int size = (m / 2) + 1;

  fftw_complex *in_freq =
      (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * size);
  fftw_complex *mat_freq =
      (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * size);
  fftw_complex *out_freq =
      (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * size);

  for (int i = 0; i < m; ++i) {
    in_fft[i] = (i < n) ? input[i] : 0.0;
    mat_fft[i] = 0.0;
    out_fft[i] = 0.0;
  }

  for (int i = 0; i < n; ++i) {
    mat_fft[i] = mat[(n - 1) - i];
  }

  for (int i = 1; i < n; ++i) {
    mat_fft[m - i] = mat[(n - 1) + i];
  }

  fftw_plan p_in = fftw_plan_dft_r2c_1d(m, in_fft, in_freq, FFTW_ESTIMATE);
  fftw_plan p_mat = fftw_plan_dft_r2c_1d(m, mat_fft, mat_freq, FFTW_ESTIMATE);
  fftw_plan p_inv = fftw_plan_dft_c2r_1d(m, out_freq, out_fft, FFTW_ESTIMATE);

  fftw_execute(p_in);
  fftw_execute(p_mat);

  for (int k = 0; k < size; ++k) {
    double a = in_freq[k][0];
    double b = in_freq[k][1];
    double c = mat_freq[k][0];
    double d = mat_freq[k][1];

    out_freq[k][0] = a * c - b * d;
    out_freq[k][1] = a * d + b * c;
  }

  fftw_execute(p_inv);

  for (int i = 0; i < n; ++i) {
    output[i] = out_fft[i] / m;
  }

  fftw_destroy_plan(p_in);
  fftw_destroy_plan(p_mat);
  fftw_destroy_plan(p_inv);

  fftw_free(in_fft);
  fftw_free(mat_fft);
  fftw_free(out_fft);
  fftw_free(in_freq);
  fftw_free(mat_freq);
  fftw_free(out_freq);
}

void ntl_fft(const NTL::vec_ZZ_p &input, const NTL::vec_ZZ_p &toep,
             NTL::vec_ZZ_p &output) {
  using namespace NTL;

  const long n = input.length();

  if (toep.length() != 2 * n - 1) {
    throw std::runtime_error("toep length must be 2n - 1");
  }

  ZZ_pX c_poly;
  ZZ_pX x_poly;

  for (long i = 0; i < n; ++i) {
    SetCoeff(c_poly, i, toep[(n - 1) - i]);
  }

  SetCoeff(c_poly, n, ZZ_p(0));

  for (long j = 1; j < n; ++j) {
    SetCoeff(c_poly, 2 * n - j, toep[(n - 1) + j]);
  }

  for (long i = 0; i < n; ++i) {
    SetCoeff(x_poly, i, input[i]);
  }

  ZZ_pX prod;
  mul(prod, c_poly, x_poly);

  output.SetLength(n);

  for (long i = 0; i < n; ++i) {
    output[i] = coeff(prod, i);
  }
}
