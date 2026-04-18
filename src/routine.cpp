module;

#include<NTL/ZZ_p.h>
#include<NTL/tools.h>
#include<NTL/vec_ZZ.h>
#include<NTL/vec_ZZ_p.h>
#include<NTL/vector.h>
#include<type_traits>

export module toeplitz;

import std;
import NTL;

/*
 * This is templated version of routine-check since we want it to be generic for different kinds of functions( different fft functions)
 * This uses NTL routine API.
 */

export namespace Toeplitz{

    template<typename Function>
    requires std::invocable<Function, long, long>
    void routine_check(const vec_ZZ_p &toep_mat, const vec_ZZ_p &input, long n, Func mul_function) noexcept(true){

        long double curr_time = GetTime();
        vec_ZZ_p mat  = mul_function(input,toep_mat, n);
        long double final_time = GetTime() - curr_time;
        global.push_back({n,final_time});
        std::cout << "[ time:  " << final_time << "]" << std::endl;

    }

} // namesapce Toeplitz
