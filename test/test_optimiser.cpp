//
// Created by visan on 5/13/23.
//
#include<catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "../src/optimiser.h"

#define private public
using namespace GeneticSimulation;

double f(double x) {
    return -x * x + x + 2;
}

TEST_CASE("Number of bits per chromosome", "[optimiser]") {
    Optimiser a(f, 20, {0, 1}, 1, 0.25, 0.01, 50);
    REQUIRE(a.get_bits_per_chromosome() == 4);
    Optimiser b(f, 20, {-1, 2}, 6, 0.25, 0.01, 50);
    REQUIRE(b.get_bits_per_chromosome() == 22);
    Optimiser c(f, 20, {-1, 1}, 2, 0.25, 0.01, 50);
    REQUIRE(c.get_bits_per_chromosome() == 8);

    Optimiser d(f, 20, {-89, 120}, 6, 0.25, 0.01, 50);
    REQUIRE(d.get_bits_per_chromosome() == 28);
}

TEST_CASE("Chromosome to domain", "[optimiser]") {
    Optimiser a(f, 20, {0, 1}, 1, 0.25, 0.01, 50);
    Organism ora(0b0101, 4);
    Organism orb(0b0000, 4);
    REQUIRE(a.to_domain(ora) == 0.3125);
    REQUIRE(a.to_domain(orb) == 0.0);


    Optimiser b(f, 20, {-1, 2}, 6, 0.25, 0.01, 50);
    Organism orc(0b0000011101001001110001, 22);
    Organism ord(0b1111110000100110000000, 22);
    REQUIRE_THAT(-0.914592, Catch::Matchers::WithinAbs(b.to_domain(orc), 0.00001));
    REQUIRE_THAT(1.954865, Catch::Matchers::WithinAbs(b.to_domain(ord), 0.00001));
}

TEST_CASE("Chromosome fitness", "[optimiser]") {
    Optimiser b(f, 20, {-1, 2}, 6, 0.25, 0.01, 50);
    Organism orc(0b0000011101001001110001, 22);
    Organism ord(0b1111110000100110000000, 22);
    REQUIRE_THAT(0.24892945, Catch::Matchers::WithinAbs(b.fitness(orc), 0.00001));
    REQUIRE_THAT(0.133367231, Catch::Matchers::WithinAbs(b.fitness(ord), 0.00001));
}

