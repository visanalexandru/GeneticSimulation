#include <iostream>
#include<cmath>
#include"matplot/matplot.h"
#include"organism.h"
#include"optimiser.h"

using namespace std;
using namespace GeneticSimulation;

double f(double x) {
    return sin(0.25 * x) + sin(M_PI * 0.1 * x) + 2;
}

double g(double x) {
    double c = cos(x * x + x + 7);
    double s = sin(x + 10);
    return c * c - s + 5;
}

int main() {
    rng.seed(time(NULL));
    Optimiser opt(g, 40, {-2, 4}, 6, 0.25, 0.3, 100000);
    double x = opt.optimise();
    std::cout << "Maximum found at x = " << x << std::endl;


    return 0;
}
