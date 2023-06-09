//
// Created by visan on 5/12/23.
//

#include "../src/defines.h"
#include<catch2/catch_test_macros.hpp>


TEST_CASE("Test fast pow", "[util]") {
    REQUIRE(GeneticSimulation::fast_pow(10, 5) == 100000);
    REQUIRE(GeneticSimulation::fast_pow(7, 11) == 1977326743);
    REQUIRE(GeneticSimulation::fast_pow(2, 30) == 1073741824);
    REQUIRE(GeneticSimulation::fast_pow(10, 0) == 1);
    REQUIRE(GeneticSimulation::fast_pow(50, 5) == 312500000);
    REQUIRE(GeneticSimulation::fast_pow(10, 2) == 100);
    REQUIRE(GeneticSimulation::fast_pow(10, 9) == 1000000000);
}

TEST_CASE("Test ceil log", "[util]") {
    REQUIRE(GeneticSimulation::ceil_log(2) == 1);
    REQUIRE(GeneticSimulation::ceil_log(1) == 1);
    REQUIRE(GeneticSimulation::ceil_log(43) == 6);
    REQUIRE(GeneticSimulation::ceil_log(64) == 6);
    REQUIRE(GeneticSimulation::ceil_log(91623) == 17);
    REQUIRE(GeneticSimulation::ceil_log(1073741824) == 30);
    REQUIRE(GeneticSimulation::ceil_log(8172464) == 23);
}


TEST_CASE("Test bitvector to string") {
    GeneticSimulation::bitvector a = 0b001010111011;
    GeneticSimulation::bitvector b = 0b1;
    GeneticSimulation::bitvector c = 0b0010111101010010101;
    REQUIRE(GeneticSimulation::bitvector_to_string(a, 12) == "001010111011");
    REQUIRE(GeneticSimulation::bitvector_to_string(b, 1) == "1");
    REQUIRE(GeneticSimulation::bitvector_to_string(c, 19) == "0010111101010010101");
}