//
// Created by visan on 5/12/23.
//

#ifndef GENETICSIMULATION_INCLUDES_H
#define GENETICSIMULATION_INCLUDES_H

#include<cstdint>

namespace GeneticSimulation {
    // A vector of 64 bits.
    typedef uint64_t bitvector;

    // An interval of the form [left,right].
    struct range {
        double left;
        double right;
    };
}

#endif //GENETICSIMULATION_INCLUDES_H
