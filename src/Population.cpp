//
// Created by marek on 2/19/17.
//

#include "Population.h"

Population::Population(std::string EStype) {
    population.resize(MU + LAMBDA);
    Chromosome::init("symAmp");

    for (int i = 0; i < MU; i++) {
        population[i] = Chromosome();
    }

    if (EStype == "comma")
        this->EStype = comma;
    else
        this->EStype = plus;
}

void Population::evolve() {
    std::mt19937 generator(std::random_device().operator()());
    unsigned generationCount = 0;
    double prevStopGen = DBL_MAX;
    double prevPrintGen = DBL_MAX;
    clock_t lastTime = 0;
    clock_t now;

    /*todo: add the preamble*/

    while (++generationCount < MAX_GEN_COUNT) {
        for (int i = MU; i < LAMBDA + MU; i++)
            population[i] = population[generator() % MU].reproduce();

        switch (EStype) {
            case plus:
                sort(population.begin(), population.end());
                break;
            case comma:
                sort(population.begin() + MU, population.end());
                for (int i = 0; i < MU; i++) {
                    population[i] = population[i+MU];
                }
                break;
            default:
                sort(population.begin(), population.end());
                break;
        }


        if (!(generationCount % PRINT_GEN) &&
                prevPrintGen > population[0].objectiveFunction()) {
            prevPrintGen = population[0].objectiveFunction() * PRINT_CHANGE;
            now = clock();
            std::cout << "Generation: " << generationCount << std::endl
                      << population[0]
                      << " time: " << double(now - lastTime) / CLOCKS_PER_SEC
                      << std::endl;
            lastTime = now;

            population[0].plot();
        }

        /*stop the evolution if the objective function doesn't change anymore*/
        if (!(generationCount % STOP_GEN)) {
            if (!(prevStopGen > population[0].objectiveFunction())) {
                break;
            }
            prevStopGen = population[0].objectiveFunction() * 
                               STOP_CHANGE;
        }
    }

}