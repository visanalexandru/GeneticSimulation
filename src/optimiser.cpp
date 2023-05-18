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

    double Optimiser::maximum_fitness(const std::vector<Organism> &organisms) const {
        double best = 0;
        for (const Organism &o: organisms) {
            best = std::max(best, fitness(o));
        }
        return best;
    }

    double Optimiser::average_fitness(const std::vector<Organism> &organisms) const {
        double sum = 0;
        for (const Organism &o: organisms) {
            sum += fitness(o);
        }
        return sum / (double) organisms.size();
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
                std::cout << next[cross[index]] << " " << next[cross[0]] << std::endl << std::endl;
            }

        }
        return next;
    }

    std::vector<Organism> Optimiser::mutation(const std::vector<Organism> &organisms, bool verbose) const {
        // We need to generate random uniform numbers in [0,1)
        std::uniform_real_distribution<> real_dist(0, 1);

        // We also need to generate integers between 0 and bits_per_chromosome-1 so that we can
        // flip random genes.
        std::uniform_int_distribution<std::mt19937::result_type> int_dist(0, bits_per_chromosome - 1);

        std::vector<Organism> mutated = organisms;

        // Indices of organisms to be mutated.
        std::vector<size_t> to_mutate;

        if (verbose) {
            std::cout << "Probability of mutation: " << mutation_probability << std::endl;
        }

        // Find organisms to be mutated.
        for (size_t i = 0; i < mutated.size(); i++) {
            double uniform = real_dist(rng);
            if (verbose) {
                std::cout << i + 1 << ": " << organisms[i] << " u = " << uniform << " ";
            }
            if (uniform < mutation_probability) {
                // Select this organism to be mutated.
                if (verbose) {
                    std::cout << "* selected";
                }
                to_mutate.push_back(i);
            }
            if (verbose) {
                std::cout << std::endl;
            }
        }
        if (verbose) {
            std::cout << std::endl;
        }

        for (size_t index: to_mutate) {
            unsigned gene = int_dist(rng);
            if (verbose) {
                std::cout << "Mutating organism " << index + 1 << ", gene " << gene << std::endl;
                std::cout << "Before: " << organisms[index] << ", ";
            }
            mutated[index].mutate(gene);
            if (verbose) {
                std::cout << "after: " << mutated[index] << std::endl;
            }
        }
        if (verbose) {
            std::cout << std::endl;
        }
        return mutated;
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
            std::cout << "After selection: " << std::endl;
            show_population(selected);
        }
        std::vector<Organism> crossed = cross_over(selected, verbose);

        if (verbose) {
            std::cout << "After crossing over: " << std::endl;
            show_population(crossed);
        }

        std::vector<Organism> mutated = mutation(crossed, verbose);

        if (verbose) {
            std::cout << "After mutation: " << std::endl;
            show_population(mutated);
        }

        // Add the fittest organism to the next generation.
        mutated.push_back(best);

        if (verbose) {
            std::cout << "Final population: " << std::endl;
            show_population(mutated);
        }


        return mutated;
    }

    double Optimiser::optimise(bool plot) {
        if (plot) {
            // Initialize the window size.
            auto w = matplot::figure(true);
            w->size(800, 800);
        }


        // Points used to plot the function.
        std::vector<double> fun_x = matplot::linspace(domain.left, domain.right, 2000);
        std::vector<double> fun_y = matplot::transform(fun_x, f);

        // These are used to plot the graph of the evolution of the average and the best fitness per iterations.
        std::vector<double> num_iter;
        std::vector<double> avg_fit;
        std::vector<double> max_fit;

        std::vector<Organism> population = initial_population();
        std::cout << "Initial population: " << std::endl;

        double best = 0;

        for (unsigned int e = 0; e < epochs; e++) {
            double max_fitness = maximum_fitness(population);
            double avg_fitness = average_fitness(population);

            if (plot) {
                // Add the current iteration to the points, paired with the best fitness and the
                // average fitness.
                num_iter.push_back(e);
                avg_fit.push_back(avg_fitness);
                max_fit.push_back(max_fitness);
            }


            // Check if the best has changed.
            if (max_fitness > best) {
                std::cout << "Max fitness: " << std::fixed << std::setprecision(10) << max_fitness << std::endl;
                std::cout << "Average fitness: " << std::fixed << std::setprecision(10) << avg_fitness << std::endl;
                best = max_fitness;

                // Plot the organisms on the graph as a scatter.
                if (plot) {
                    std::vector<double> points;
                    std::vector<double> fit;
                    for (const Organism &o: population) {
                        points.push_back(to_domain(o));
                        fit.push_back(fitness(o));
                    }
                    // Plot the function.
                    auto function_plot = matplot::plot(fun_x, fun_y);
                    // Set the line width.
                    function_plot->line_width(3);
                    // Keep the function line, add the scatter.
                    matplot::hold(matplot::on);

                    auto scatter = matplot::scatter(points, fit, 10);
                    // Fill the dots.
                    scatter->marker_face(true);
                    matplot::hold(matplot::off);
                    // Show the plot.
                    matplot::show();
                }

            }

            if (e == 0) {
                // If the current epoch is the first one, enable verbose output.
                population = next_generation(population, true);
            } else {
                population = next_generation(population, false);
            }
        }


        // Now that the optimisation has ended, plot the average and best fitness plots.

        if (plot) {
            // Plot the average line.
            auto avg_plot = matplot::plot(num_iter, avg_fit);
            avg_plot->display_name("average");
            avg_plot->line_width(3);
            matplot::hold(matplot::on);

            // Plot the max line.
            auto max_plot = matplot::plot(num_iter, max_fit);
            max_plot->display_name("maximum");
            max_plot->line_width(3);
            matplot::hold(matplot::off);


            // Enable the legend and set the position of the label.
            auto legend = matplot::legend();
            legend->location(matplot::legend::general_alignment::bottomright);

            matplot::show();
        }

        return to_domain(fittest(population));
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