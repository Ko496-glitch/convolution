#include<NTL/ZZ_p.h>
#include<NTL/tools.h>
#include<NTL/vec_ZZ.h>
#include<NTL/vec_ZZ_p.h>
#include<NTL/vector.h>

constexpr const long PRIME_BIT  = 60;
constexpr const long Err_bound =  80;
using namespace NTL;

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
        tope_mat = input[i];
        res[i] = tope_mat[i];
    }
    return res;
}

/* optimized mutliplation of tope_mat * input vector. O(n log n). Would be benchmark against mul_norm */

vec_zz_p cooley_mul(const vec_ZZ_p &input, const vec_ZZ_p &tope_mat,int n ){
    vec_zz_p res;

    for(long i{};i<n;++i){

    }



    return res;
}

int main(){

  ZZ p;
  GenPrime(p ,PRIME_BIT ,Err_bound);
  ZZ_p::init(p);
  long n = RandomBnd(1024) + 1;
  SetSeed(to_ZZ(time(nullptr)));

  vec_ZZ_p input;

  input.SetLength(n);

  for(long long i{};i< n; ++i){
    random(input[i]);
  }

  vec_ZZ_p toep_mat;

  toep_mat.SetLength(2*n-1);

  for(long i{};i<2*n-1;++i){
    random(toep_mat[i]);
  }

  return 0;
}
