module;

#include <NTL/vec_ZZ_p.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include<concepts>
#include<type_traits>

export module Toeplitz;

import std;
import NTL;


export namespace Toeplitz::Utils{

    template<typename Function>
    requires std::invocable<Function,long, long>
    void print_matrix(long n, Function toep_mat) noexcept(false){
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

}// namespace Toeplitz
