#include "../../include/routine.hpp"
#include "../../include/print.hpp"
#include "../../include/toeplitz.hpp"
#include <NTL/ZZ_p.h>
#include <fstream>
#include <iostream>
using namespace std;
using namespace NTL;

namespace toeplitz {
std::vector<std::pair<long, long double>> global;
}

int main() {
  NTL::ZZ_p::init(NTL::ZZ(786433));
  long start = 500;
  long end = 3000;
  long step = 200;

  while (start <= end) {
    vec_ZZ_p input;
    input.SetLength(start);
    for (long i = 0; i < start; ++i)
      random(input[i]);

    vec_ZZ_p toep_vec;
    generate_toeplitz_data(toep_vec, start);

    toeplitz::routine_check(toep_vec, input, start, mul_norm_);
    print_matrix(toep_vec, start);
    start += step;
    std::ofstream file("data.csv");
    file << "n,time\n";

    for (const auto &[n, t] : toeplitz::global) {
      file << n << "," << t << "\n";
    }

    file.close();
  }
}