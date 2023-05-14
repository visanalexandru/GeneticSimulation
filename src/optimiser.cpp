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

    std::vector<Organism> Optimiser::selection(const std::vector<Organism> &organisms, bool verbose) const {
        std::vector<double> fitness_score(organisms.size());
        double total = 0, last = 0;

        // Precompute fitness scores and the total sum.
        for (size_t i = 0; i < organisms.size(); i++) {
            fitness_score[i] = fitness(organisms[i]);
            total += fitness_score[i];
        }

        // Generate the intervals.
        std::vector<double> intervals;
        for (size_t i = 0; i < organisms.size(); i++) {
            double probability = fitness_score[i] / total;
            intervals.push_back(last + probability);
            last += probability;
            if (verbose) {
                std::cout << "Organism " << i + 1 << " has a probability of " << fitness_score[i] / total << std::endl;
            }
        }

        if (verbose) {
            std::cout << std::endl;
            std::cout << "Probability intervals: " << std::endl;
            for (double x: intervals) {
                std::cout << x << " ";
            }
            std::cout << std::endl;
        }

        /* Then generate organism.size()-1 numbers in [0,1).
         * For each generated number, we need to find out which interval
         * it resides in. So for a number x, we need to find the smallest y such that
         * y>x.
         */

        std::vector<Organism> selected;
        for (size_t i = 0; i < organisms.size() - 1; i++) {
            // Random uniform number int [0 , 1).
            std::uniform_real_distribution<> dist(0, 1);
            double uniform = dist(rng);

            size_t index = std::upper_bound(intervals.begin(), intervals.end(), uniform) - intervals.begin();

            selected.push_back(organisms[index]);
            if (verbose) {
                std::cout << "u = " << uniform << " we choose the organism " << index + 1 << std::endl;
            }
        }

        if (verbose) {
            std::cout << std::endl;
        }

        return selected;
    }

    void Optimiser::show_population(const std::vector<Organism> &population) const {
        size_t index = 1;
        for (const Organism &organism: population) {
            std::cout << index << ": " << organism << " ";
            std::cout << "x = " << to_domain(organism) << " ";
            std::cout << "f = " << fitness(organism) << std::endl;
            index++;
        }
        std::cout << std::endl;
    }

    Organism Optimiser::fittest(const std::vector<Organism> &organisms) const {
        double best = 0;
        Organism to_return = organisms[0];
        for (const Organism &o: organisms) {
            double ft = fitness(o);
            if (ft > best) {
                to_return = o;
                best = ft;
            }
        }
        return to_return;
    }

    std::vector<Organism> Optimiser::cross_over(const std::vector<Organism> &organisms, bool verbose) const {
        // Indices of organisms that will be crossed-over.
        std::vector<size_t> cross;

        // The next population.
        std::vector<Organism> next = organisms;

        // We need to generate split points.
        std::uniform_int_distribution<std::mt19937::result_type> distribution(0, bits_per_chromosome - 1);

        if (verbose) {
            std::cout << "Cross probability: " << cross_probability << std::endl;
        }

        // Select organisms to be crossed over.
        for (size_t index = 0; index < organisms.size(); index++) {
            // Generate a uniform number in [0,1)
            std::uniform_real_distribution<> dist(0, 1);
            double uniform = dist(rng);

            if (verbose) {
                std::cout << index + 1 << ": " << organisms[index] << " u= " << uniform;
            }

            if (uniform < cross_probability) {
                if (verbose) {
                    std::cout << " * selected";
                }
                cross.push_back(index);
            }

            if (verbose) {
                std::cout << std::endl;
            }
        }

        if (verbose) {
            std::cout << std::endl;
        }

        // Apply the cross-over operation.
        size_t index = 0;
        while (index < cross.size() && index + 1 < cross.size()) {
            // Generate a random split point between 0 and bits_per_chromosome-1.
            unsigned int split_point = distribution(rng);

            if (verbose) {
                std::cout << "Combining chromosome " << cross[index] + 1 << " with chromosome " << cross[index + 1] + 1
                          << std::endl;
                std::cout << next[cross[index]] << " " << next[cross[index + 1]] << " ";
                std::cout << "split point: " << split_point << std::endl;
            }

            next[cross[index]].cross(next[cross[index + 1]], split_point);
            if (verbose) {
                std::cout << "Result: ";
                std::cout << next[cross[index]] << " " << next[cross[index + 1]] << std::endl;
            }
            index += 2;
        }

        // There might be one more chromosome without a pair.
        if (index < cross.size()) {
            // Pair it with the first one.
            unsigned int split_point = distribution(rng);

            if (verbose) {
                std::cout << "Combining chromosome " << cross[index] + 1 << " with chromosome " << cross[0] + 1
                          << std::endl;
                std::cout << next[cross[index]] << " " << next[cross[0]] << " ";
                std::cout << "split point: " << split_point << std::endl;
            }

            next[cross[index]].cross(next[cross[0]], split_point);

            if (verbose) {
                std::cout << "Result: ";
                std::cout << next[cross[index]] << " " << next[cross[0]] << std::endl;
            }

        }
        return next;
    }


    std::vector<Organism> Optimiser::next_generation(const std::vector<Organism> &organisms, bool verbose) const {
        if (organisms.empty()) {
            return organisms;
        }
        if (verbose) {
            show_population(organisms);
        }
        // Find the fittest organism, so that it is passed in the next generation.
        Organism best = fittest(organisms);
        std::vector<Organism> selected = selection(organisms, verbose);

        if (verbose) {
            std::cout << "Fittest: " << fitness(best) << std::endl;
        }

        if (verbose) {
            std::cout << "After selection: " << std::endl;
            show_population(selected);
        }
        std::vector<Organism> crossed = cross_over(selected, verbose);


        return selected;
    }

    double Optimiser::optimise() {
        std::vector<Organism> population = initial_population();
        std::cout << "Initial population: " << std::endl;
        next_generation(population, true);
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