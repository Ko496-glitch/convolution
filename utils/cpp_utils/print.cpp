#include <NTL/vec_ZZ_p.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>


void print_matrix(const NTL::vec_ZZ_p &toep_mat, long n) {
    const int width = 30;
    /* 
    auto print_separator = [&]() {
        std::cout << "+";
        for(long i = 0; i < n; ++i)
            std::cout << std::string(width, '-') << "+";
        std::cout << "\n";
    };


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
        }*/
     std::cout << "\n--- For (n=" << n << " ) ---\n";
}