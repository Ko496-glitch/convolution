#include "../../include/routine.hpp"
#include "../../include/print.hpp"
#include "../../include/toeplitz.hpp"
#include <NTL/ZZ_p.h>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;
using namespace NTL;

std::vector<long double> fftw_times_c2c;
std::vector<long double> fftw_times_r2c;
namespace toeplitz {
std::vector<std::pair<long, long double>> global;
}

int main() {
  NTL::ZZ_p::init(NTL::ZZ(786433));

  long start = 1000;
  long end = 10000;
  long step = 1000;

  while (start <= end) {
    std::cout << "\n--- For (n = " << start << ") ---\n";
    vec_ZZ_p input;
    input.SetLength(start);
    for (long i = 0; i < start; ++i)
      random(input[i]);

    vec_ZZ_p toep_vec;
    long double toep_start = GetTime();
    generate_toeplitz_data(toep_vec, start);

    long double toep_end = GetTime() - toep_start;
    std::cout << "Time taken to generate random toeplitz =" << toep_end
              << std::endl;

    toeplitz::routine_check(toep_vec, input, start, mul_norm_);

    std::vector<double> input_d(start);
    std::vector<double> mat_d(2 * start - 1);
    std::vector<double> output_d(start);

    for (long i = 0; i < start; ++i)
      input_d[i] = static_cast<double>(NTL::conv<long>(rep(input[i])));

    for (long i = 0; i < 2 * start - 1; ++i)
      mat_d[i] = static_cast<double>(NTL::conv<long>(rep(toep_vec[i])));

    int trials = 50;

    long double time = 0.0;
    for (int i = 0; i < trials; ++i) {
      long double t0 = GetTime();
      fftw_c2c(input_d.data(), mat_d.data(), output_d.data(), start);
      time += GetTime() - t0;
    }

    long double avg_c2c = time / trials;
    fftw_times_c2c.push_back(avg_c2c);
    std::cout << "Time taken just to run fftw_complex = " << avg_c2c
              << std::endl;

    time = 0.0;
    for (int i = 0; i < trials; ++i) {
      long double t1 = GetTime();
      fftw_real(input_d.data(), mat_d.data(), output_d.data(), start);
      time += GetTime() - t1;
    }

    long double avg_r2c = time / trials;
    fftw_times_r2c.push_back(avg_r2c);
    std::cout << "Time taken just to run fftw_real = " << avg_r2c << std::endl;
    start += step;
  }
  std::ofstream file("build/data.csv");

  file << "n,naive,fftw_c2c,fftw_r2c,"
          "naive_over_nlogn,fftw_c2c_over_nlogn,fftw_r2c_over_nlogn,"
          "naive_over_n2\n";

  for (size_t i = 0; i < toeplitz::global.size(); ++i) {
    auto [n, naive_time] = toeplitz::global[i];

    long double n_ld = static_cast<long double>(n);
    long double nlogn = n_ld * std::log2(n_ld);
    long double n2 = n_ld * n_ld;

    long double fftw_c2c_time = fftw_times_c2c[i];
    long double fftw_r2c_time = fftw_times_r2c[i];

    file << n << "," << naive_time << "," << fftw_c2c_time << ","
         << fftw_r2c_time << "," << naive_time / nlogn << ","
         << fftw_c2c_time / nlogn << "," << fftw_r2c_time / nlogn << ","
         << naive_time / n2 << "\n";
  }

  file.close();
}