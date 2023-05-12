//
// Created by visan on 5/12/23.
//
#include<catch2/catch_test_macros.hpp>
#include "../src/organism.h"

TEST_CASE("Crossing testing", "[organism]") {
    GeneticSimulation::bitvector a1 = 0b010110110101;
    GeneticSimulation::bitvector b1 = 0b110010101100;


    GeneticSimulation::Organism a(a1,12), b(b1,12);

    SECTION("i = 4") {
        a.cross(b, 4);
        GeneticSimulation::bitvector a2 = 0b110010110101;
        GeneticSimulation::bitvector b2 = 0b010110101100;
        REQUIRE(a.get_chromosome() == a2);
        REQUIRE(b.get_chromosome() == b2);
    }SECTION("i = 0") {
        a.cross(b, 0);
        GeneticSimulation::bitvector a2 = 0b110010101101;
        GeneticSimulation::bitvector b2 = 0b010110110100;
        REQUIRE(a.get_chromosome() == a2);
        REQUIRE(b.get_chromosome() == b2);
    }SECTION("i = 4") {
        a.cross(b, 4);
        GeneticSimulation::bitvector a2 = 0b110010110101;
        GeneticSimulation::bitvector b2 = 0b010110101100;
        REQUIRE(a.get_chromosome() == a2);
        REQUIRE(b.get_chromosome() == b2);
    }

    SECTION("i = 8") {
        a.cross(b, 8);
        GeneticSimulation::bitvector a2 = 0b110110110101;
        GeneticSimulation::bitvector b2 = 0b010010101100;
        REQUIRE(a.get_chromosome() == a2);
        REQUIRE(b.get_chromosome() == b2);
    }

    SECTION("i = 11") {
        a.cross(b, 11);
        REQUIRE(a.get_chromosome() == a1);
        REQUIRE(b.get_chromosome() == b1);
    }
}


TEST_CASE("Mutation testing", "[organism]") {

    GeneticSimulation::bitvector a = 0b10111010001101010;
    GeneticSimulation::Organism organism(a,17);

    SECTION("i = 3") {
        organism.mutate(3);
        REQUIRE(organism.get_chromosome() == 0b10111010001100010);
    }

    SECTION("i = 12") {
        organism.mutate(12);
        REQUIRE(organism.get_chromosome() == 0b10110010001101010);
    }
    SECTION("i = 16, i = 12, i = 12") {
        organism.mutate(16);
        organism.mutate(12);
        organism.mutate(12);
        REQUIRE(organism.get_chromosome() == 0b00111010001101010);
    }


    SECTION("i = 17") {
        organism.mutate(17);
        REQUIRE(organism.get_chromosome() == a);
    }

    SECTION("i = 20") {
        organism.mutate(20);
        REQUIRE(organism.get_chromosome() == a);
    }


}
