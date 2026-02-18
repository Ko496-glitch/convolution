#include<NTL/ZZ_p.h>
#include<NTL/tools.h>
#include<NTL/vec_ZZ.h>
#include<NTL/vec_ZZ_p.h>
#include<NTL/vector.h>
#include<iostream>
#include<sstream>
#include<iomanip>
#include<string>

constexpr const long PRIME_BIT  = 60;
constexpr const long ERR_BOUND =  80;
using namespace NTL;

long long total_time{};
std::vector<std::pair<long,long double>>global;
/* O(n^2) compute for toeplitz matrix with input vector Benchmarking against Cooley method */
/* from 1d -> 2d we use V[(n-1) + ( j- i) * xj)] j: 0 -> n */

vec_ZZ_p mul_norm(const vec_ZZ_p &input, const  vec_ZZ_p &tope_mat, long n){
    vec_ZZ_p res;
    res.SetLength(n);

    for(long i{};i<n;++i){
        ZZ_p sum{};
        for(long j{};j < n;++j){
            long v  = (n - 1) + (j - i);
            ZZ_p prod;
            mul(prod, tope_mat[v], input[j]);
            add(sum, sum, prod);
        }
        res[i] = sum;
    }
    return res;
}

#if 0
/* optimized mutliplation of tope_mat * input vector. O(n log n). Would be benchmark against mul_norm */
vec_ZZ_p cooley_mul(const vec_ZZ_p &input, const vec_ZZ_p &tope_mat, long n ){
    vec_zz_p res;

    for(long i{};i<n;++i){

    }
    return res;
}

#endif

/* Function the benchmark the time and the performance */
/* generic so we can pass any type of Multiplication Function */

template<typename Func>
void routine_check(const vec_ZZ_p &toep_mat, const vec_ZZ_p &input, long n, Func mul_function){

    long double curr_time = GetTime();
    vec_ZZ_p mat  = mul_function(input,toep_mat, n);
    long double final_time = GetTime() - curr_time;
    global.push_back({n,final_time});
    std::cout << "[ time:  " << final_time << "]" << std::endl;
}


void print_matrix(const vec_ZZ_p &toep_mat, long n) {
    const int width = 30;

    auto print_separator = [&]() {
        std::cout << "+";
        for(long i = 0; i < n; ++i)
            std::cout << std::string(width, '-') << "+";
        std::cout << "\n";
    };

    std::cout << "\n--- Toeplitz Matrix Verification (n=" << n << ") ---\n";

    print_separator();

    for(long i = 0; i < n; ++i) {
        std::cout << "|";

        for(long j = 0; j < n; ++j) {
            long index = (n-1) + (j-i);

            std::ostringstream oss;
            oss << toep_mat[index];
            std::string s = oss.str();

            std::cout << std::setw(width) << s << "|";
        }

        std::cout << "\n";
        print_separator();
    }
}

int main(){

  ZZ p;
  GenPrime(p ,PRIME_BIT ,ERR_BOUND);
  ZZ_p::init(p);
  long t = 8;
  SetSeed(to_ZZ(time(nullptr)));



  while(t--){

      long n = (RandomBnd(10)+1) * 100;
      vec_ZZ_p input;

      input.SetLength(n);

      for(long i{};i< n; ++i){
        random(input[i]);
      }

      vec_ZZ_p toep_mat;

      toep_mat.SetLength(2*n-1);

      for(long i{};i<2*n-1;++i){
        random(toep_mat[i]);
      }
      print_matrix(toep_mat, n);
      routine_check(toep_mat,input, n, mul_norm);

  }

  std::uint8_t total_time{};

  for(auto&[n,final_time]:global){
      total_time += final_time;
      std::cout << "for n: " << n << " Time taken: " << final_time << std::endl;

  }

  std::cout << "Total time taken: " << total_time << std::endl;

  return 0;
}
