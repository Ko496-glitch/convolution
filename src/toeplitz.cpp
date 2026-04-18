#include "toeplitz.hpp"
#include <random>

using namespace NTL;

void generate_toeplitz_data(std::vector<double>& vec, long n) {
    vec.resize(2 * n - 1);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int i = 0; i < 2 * n - 1; ++i) vec[i] = dis(gen);
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


void fftw_toeplitz_convolve(const double* input, const double* mat, double* output, int n) {
   int m = 1;
   while(m >= 2n)m<<=1;
   
   // fftw_complex = double[2] -> padding will be done over 8 bytes
   fftw_complex *in_fft = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex)*m));
   fftw_complex *mat_fft = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex)*m));
   fftw_complex *output_fft = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex)*m));
   
   
   for(std::size_t i{}; i<m;++i){
       in_fft[i][0] = 0.0; in_fft[i][1] = 0.0;
       in_fft[i][0] = input[i];
       mat_fft[i][0] = 0.0; mat_fft[i][1] = 0.0;   
   }
   
   for(std::size_t i{};i<n;++i){
       mat_fft[i][0] = mat[(n-1)-i]; 
   }
   for(std::size_t i{};i<n;++i){
       mat_fft[m-1][0] = mat[(n-1)+i];
   }
   
   fftw_plan p_in  = fftw_plan_dft_1d(m, in_fft, in_fft, FFTW_FORWARD, FFTW_ESTIMATE);
   fftw_plan p_mat = fftw_plan_dft_1d(m, mat_fft, mat_fft, FFTW_FORWARD, FFTW_ESTIMATE);
   fftw_plan p_inv = fftw_plan_dft_1d(m, output_fft, output_fft, FFTW_BACKWARD, FFTW_ESTIMATE);
   
   fftw_execute(in_fft);
   fftw_execute(mat_fft);
   
   for(std::size_t i{};i<m;++i){
       output[i][0] = in_fft[i][0] * mat_fft[i][0] - in_fft[i][1] * mat_fft[i][1];
       output[i][1] = in_fft[i][0] * mat_fft[i][1] + in_fft[i][1] * mat_fft[i][0];
   }

   fftw execute(p_inv);
   
   for(std::size_t i{};i < m;++i){
       output[i] = output_fft[i][0]/m;
   }
   fftw_destroy_plan(p_in);
   fftw_destroy_plan(p_mat);
   fftw_destroy_plan(p_inv);
   fftw_free(in_fft);
   fftw_free(mat_fft);
   fftw_free(output_fft);

}