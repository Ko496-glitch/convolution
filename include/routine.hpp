#pragma once

#include <iostream>
#include <utility>
#include <vector>

#include <NTL/tools.h>
#include <NTL/vec_ZZ_p.h>

using namespace NTL;

namespace toeplitz {

extern std::vector<std::pair<long, long double>> global;

template <typename Func>
void routine_check(const vec_ZZ_p &toep_mat, const vec_ZZ_p &input, long n,
                   Func mul_function) {
  static long double prev_time = -1.0;

  long double curr_time = GetTime();

  vec_ZZ_p mat = mul_function(input, toep_mat, n);

  long double final_time = GetTime() - curr_time;

  global.push_back({n, final_time});

  std::cout << "n = " << n << ", time = " << final_time;

  if (prev_time > 0.0) {
    long double percent = ((final_time - prev_time) / prev_time) * 100.0;
    std::cout << ", increase = " << percent << "%";
  }

  std::cout << std::endl;

  prev_time = final_time;
}

} // namespace toeplitz