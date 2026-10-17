#include<iostream>
#include<vector>
#include<algorithm>

namespace ntt{
    std::uint64_t reverse_bits(std::uint64_t index, unsigned bits){
        
        if(bits == 0)return 0;
        return __builtin_bitreverse64(index) >> (64 - bits);
    }

    void bit_reversal(std::vector<std::uint64_t> &vec){
        const std::size_t n = vec.size();
        if(n == 0 || (n &(n -1)) !=0){
            throw std::invalid_argument("NTT length must be a power of two");
        }
    }
        unsigned bits = 0;
        for(std::size_t i{n}; i > 1;i >>=1){
            ++bits;
        }

        for(std::size_t i{};i < n; ++i){
            const std::size_t j = reverse_bits(i,bits);
            if(i < j){
                std::swap(vec[i], vec[j]);
            }
        }
        
} // namespace ntt 



