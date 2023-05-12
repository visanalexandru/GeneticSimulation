#include <iostream>
#include"matplot/matplot.h"
#include"organism.h"
using namespace std;

int main() {
    GeneticSimulation::Organism organism(1,10);
    organism.mutate(1);
    cout<<organism.get_chromosome();
    return 0;
}
