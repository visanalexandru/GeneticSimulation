//
// Created by visan on 5/12/23.
//

#include "optimiser.h"

namespace GeneticSimulation {
    Optimiser::Optimiser(std::function<double(double)> _function,
                         unsigned int _population_size,
                         GeneticSimulation::range _domain,
                         unsigned int _precision,
                         double _cross_probability,
                         double _mutation_probability,
                         unsigned int _epochs) :
            f(std::move(_function)),
            population_size(_population_size),
            domain(_domain),
            precision(_precision),
            cross_probability(_cross_probability),
            mutation_probability(_mutation_probability),
            epochs(_epochs) {

        // The number of discrete points in the domain.
        // The formula is : (b-a) * 10^p
        unsigned int num_discrete = ceil((domain.right - domain.left) * fast_pow(10, precision));

        // Now compute the number of bits needed to represent a chromosome.
        // A chromosome is a point in the set of discrete points.
        bits_per_chromosome = ceil_log(num_discrete);

        step_size = (domain.right - domain.left) / (1 << bits_per_chromosome);
    }


    std::vector<Organism> Optimiser::initial_population() const {
        std::vector<Organism> result;
        for (unsigned i = 0; i < population_size; i++) {
            result.push_back(Organism::random_organism(bits_per_chromosome));
        }
        return result;
    }


    double Optimiser::optimise() {
        std::vector<Organism> population = initial_population();
        std::cout << "Initial population: " << std::endl;
        for (const Organism &organism: population) {
            std::cout << bitvector_to_string(organism.get_chromosome(), bits_per_chromosome) << " ";
            std::cout << "x = " << organism.get_chromosome() * step_size + domain.left << " ";
            std::cout << "f = " << fitness(organism) << std::endl;
        }
        return 0;
    }

    unsigned int Optimiser::get_bits_per_chromosome() const {
        return bits_per_chromosome;
    }

    double Optimiser::fitness(const GeneticSimulation::Organism &organism) const {
        return f(to_domain(organism));
    }

    double Optimiser::to_domain(const GeneticSimulation::Organism &organism) const {
        auto chr = (double) organism.get_chromosome();
        return chr * step_size + domain.left;
    }
}