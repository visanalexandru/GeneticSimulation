//
// Created by visan on 5/12/23.
//

#ifndef GENETICSIMULATION_ORGANISM_H
#define GENETICSIMULATION_ORGANISM_H

#include"defines.h"
#include<iostream>


namespace GeneticSimulation {
    /*
     * This class represents an organism that carries a chromosome that can be
     * represented using a maximum of 64 bits.
     */
    class Organism {
    private:
        // The chromosome.
        bitvector chromosome;

        // The size of the chromosome, in bits.
        unsigned int chromosome_size;

    public:
        /*
         * Create a new organism that carries the given chromosome.
         */
        explicit Organism(bitvector _chromosome, unsigned int _chromosome_size);

        /*
         * Returns the chromosome of the organism.
         */
        bitvector get_chromosome() const;

        /*
         * Sets the new value of the chromosome.
         */
        void set_chromosome(bitvector new_chromosome);

        /*
         * Returns the size of the chromosome.
         */
        unsigned int get_chromosome_size() const;

        /*
         * Sets the new value of the chromosome size.
         */
        void set_chromosome_size(unsigned int new_chromosome_size);

        /*
         * Create a new organism by crossing the two organisms.
         * A1 :a0, a1, a2 .... an
         * B1: b0, b1, b2 .... bn
         *
         * A2 : a0, a1 ... ai, bi+1, bi+2 ... bn
         * B2 : b0, b1 ... bi, ai+1, ai+2 ... an
         * The resulting chromosome: a0, a1 ... ai, b0, b1 ... bn
         */
        void cross(Organism &other, unsigned int i);

        /*
         * Mutates the chromosome by flipping the ith bit.
         */
        void mutate(unsigned int i);

        /*
         * This static function generates a random organism with the given chromosome size.
         */
        static Organism random_organism(unsigned int chromosome_size);

        /*
         * Logging information to stdout.
         */
        friend std::ostream &operator<<(std::ostream &os, const Organism &organism);
    };


}

#endif //GENETICSIMULATION_ORGANISM_H
