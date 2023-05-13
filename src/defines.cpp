//
// Created by visan on 5/12/23.
//
#include"defines.h"

namespace GeneticSimulation {
    std::mt19937 rng;

    unsigned int fast_pow(unsigned int base, unsigned int power) {
        unsigned int result = 1;
        for (unsigned int i = 1; i <= power; i = i << 1) {
            if ((power & i) != 0) {
                result *= base;
            }
            base = base * base;
        }
        return result;
    }

    unsigned int ceil_log(unsigned int x) {
        unsigned int num_bits = 1;
        while ((1 << num_bits) < x) {
            num_bits++;
        }
        return num_bits;
    }

    unsigned int random_bitvector(unsigned int num_bits) {
        unsigned int maximum = (1 << num_bits) - 1;
        std::uniform_int_distribution<std::mt19937::result_type> distribution(0, maximum);
        return distribution(rng);
    }

    std::string bitvector_to_string(bitvector vector, unsigned int num_bits) {
        std::string result;
        for (long long x = num_bits - 1; x >= 0; x--) {
            if ((vector & (1 << x)) != 0) {
                result.push_back('1');
            } else {
                result.push_back('0');
            }
        }
        return result;
    }
}