#include<NTL/ZZ_p.h>
#include<NTL/tools.h>
#include<NTL/vec_ZZ.h>
#include<NTL/vec_ZZ_p.h>
//#include<NTL/vector.h>

using namespace NTL;

int main(){


  ZZ p;
  long  k = 60;
  long err = 80;
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
