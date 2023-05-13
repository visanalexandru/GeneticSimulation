//
// Created by visan on 5/12/23.
//

#ifndef GENETICSIMULATION_OPTIMISER_H
#define GENETICSIMULATION_OPTIMISER_H

#include<functional>
#include<cmath>
#include<vector>
#include<iostream>
#include "organism.h"
#include "defines.h"

namespace GeneticSimulation {
    /*
     * This class represents the optimiser of a given real function over a given range.
     */
    class Optimiser {
    private:
        /*
         * The function to optimise.
         */
        std::function<double(double)> f;

        /*
         * The size of the first generation.
         */
        unsigned int population_size;

        /*
         * The domain in which we search for the value x for which f(x) is maximum.
         */
        range domain;

        /*
         * The level of precision we use to find the maximum.
         * We will convert the given domain to a set of discreet intervals of length (b-a) * 10^precision.
         */
        unsigned int precision;

        /*
         * The probability of an organism to be selected for crossing.
         */
        double cross_probability;

        /*
         * The probability of an organism to mutate.
         */
        double mutation_probability;

        /*
         * The number of epochs the optimiser will simulate.
         */
        unsigned int epochs;

        /*
         * The number of bits needed to represent a chromosome.
         */
        unsigned int bits_per_chromosome;

        /*
         * The size of a discrete interval.
         */
        double step_size;

        /*
         * Generates the initial population. It consists of randomly generated organisms.
         */
        std::vector<Organism> initial_population() const;


    public:
        Optimiser(std::function<double(double)> _function,
                  unsigned int _population_size,
                  range _domain,
                  unsigned int _precision,
                  double _cross_probability,
                  double _mutation_probability,
                  unsigned int _epochs);


        /*
         * This method approximates x such that f(x) is maximal.
         */
        double optimise();

        /*
         * Returns the number of bits needed to represent a chromosome.
         */
        unsigned int get_bits_per_chromosome() const;

        /*
         * Returns the fitness score of the given organism.
         */
        double fitness(const Organism &organism) const;

        /*
         * Converts the given organism to a number in the given domain.
         */
        double to_domain(const Organism &organism) const;

    };
}

#endif //GENETICSIMULATION_OPTIMISER_H
