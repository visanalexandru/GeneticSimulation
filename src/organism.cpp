//
// Created by visan on 5/12/23.
//

#include "organism.h"

namespace GeneticSimulation {
    Organism::Organism(bitvector _chromosome, unsigned int _chromosome_size) : chromosome(_chromosome),
                                                                               chromosome_size(_chromosome_size) {

    }


    bitvector Organism::get_chromosome() const {
        return chromosome;
    }

    void Organism::set_chromosome(bitvector new_chromosome) {
        chromosome = new_chromosome;
    }

    unsigned int Organism::get_chromosome_size() const {
        return chromosome_size;
    }

    void Organism::set_chromosome_size(unsigned int new_chromosome_size) {
        chromosome_size = new_chromosome_size;
    }


    void Organism::cross(Organism &other, unsigned int i) {
        // It doesn't make sense for i to be more than chromosome_size-1.
        if (i >= chromosome_size)
            return;

        // Mask bits 0,1 ... i.
        bitvector mask = (1ll << (i + 1)) - 1;
        // Mask bits i+1, i+2 ... n
        bitvector mask_n = ~mask;

        bitvector a_prefix = chromosome & mask;
        bitvector a_suffix = chromosome & mask_n;

        bitvector b_prefix = other.chromosome & mask;
        bitvector b_suffix = other.chromosome & mask_n;

        chromosome = a_prefix | b_suffix;
        other.set_chromosome(b_prefix | a_suffix);
    }

    void Organism::mutate(unsigned int i) {
        // It doesn't make sense for i to be more than chromosome_size-1.
        if (i >= chromosome_size)
            return;
        bitvector bit = 1ll << i;
        chromosome ^= bit;
    }

    Organism Organism::random_organism(unsigned int chromosome_size) {
        return Organism(random_bitvector(chromosome_size), chromosome_size);
    }

    std::ostream &operator<<(std::ostream &os, const Organism &organism) {
        os << bitvector_to_string(organism.chromosome, organism.chromosome_size);
        return os;
    }
}

