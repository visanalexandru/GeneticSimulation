//
// Created by visan on 5/12/23.
//

#ifndef GENETICSIMULATION_INCLUDES_H
#define GENETICSIMULATION_INCLUDES_H

#include<cstdint>
#include<random>
#include<string>

namespace GeneticSimulation {
    // A vector of 64 bits.
    typedef uint64_t bitvector;

    // An interval of the form [left,right].
    struct range {
        double left;
        double right;
    };

    // Computes base^power.
    unsigned int fast_pow(unsigned int base, unsigned int power);

    /* Computes ceil(log2(x)). In other words, in counts how many bits it is needed to represent x
     * elements.
     */
    unsigned int ceil_log(unsigned int x);

    // The random number generator;
    extern std::mt19937 rng;

    // Generates a random array of bits of the given size.
    unsigned int random_bitvector(unsigned int num_bits);

    // Converts the given bitvector to a string of ones and zeroes.
    std::string bitvector_to_string(bitvector vector, unsigned int num_bits);
}

#endif //GENETICSIMULATION_INCLUDES_H
