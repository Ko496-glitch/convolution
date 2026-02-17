#include<NTL/ZZ_p.h>
#include<NTL/tools.h>
#include<NTL/vec_ZZ.h>
#include<NTL/vec_ZZ_p.h>
#include<NTL/vector.h>

constexpr const PRIME_BIT  = 60;
constexpr const Err_bound =  80;
using namespace NTL;

/* O(n^2) compute for toeplitz matrix with input vector Benchmarking against Cooley method */

/*
void mul_norm(vec_ZZ_p &input, vec_ZZ_p &toep_mat, int n){

    for(long i{};i<n;++i){

    }

}
*/

int main(){

  ZZ p;
  GenPrime(p ,k ,err);
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
