//
// Created by marek on 2/19/17.
//

#include "Population.h"

Population::Population() {
    population.resize(MU + LAMBDA);
    Chromosome::init();

    for (int i = 0; i < MU; i++) {
        population[i] = new Chromosome;
    }
}

const Chromosome Population::getResult() const {
    return *population[0];
}

void Population::evolve() {
    unsigned long generationCount = 0;
    clock_t lastTime = 0;
    clock_t now;

    while (population[0]->objectiveFunction() > 2) {
        for (int i = 0; i < LAMBDA; i++)
            population[i + LAMBDA] = population[i]->reproduce();

        sort(population.begin(), population.end(), Chromosome::compare);

        for (int i = MU; i < MU + LAMBDA; i++)
            delete population[i];

        if ((++generationCount % 100) == 0) {
            now = clock();
            std::cout << "Generation: " << generationCount << std::endl
                      << " result: " << population[0]->objectiveFunction()
                      << std::endl
                      << " R1: " << population[0]->getGenotype().R1
                      << std::endl
                      << " R2: " << population[0]->getGenotype().R2
                      << std::endl
                      << " Re: " << population[0]->getGenotype().Re
                      << std::endl
                      << " Rg: " << population[0]->getGenotype().Rg
                      << std::endl
                      << " time: " << double(now - lastTime) / CLOCKS_PER_SEC
                      << std::endl;
            lastTime = now;
        }

        if ((generationCount % 2000) == 0)
            population[0]->plot();

    }

    for (int i = 0; i < MU; i++)
        delete population[i];

    std::cout << "Generation: " << generationCount << std::endl
              << " result: " << population[0]->objectiveFunction()
              << std::endl
              << " R1: " << population[0]->getGenotype().R1
              << std::endl
              << " R2: " << population[0]->getGenotype().R2
              << std::endl
              << " Re: " << population[0]->getGenotype().Re
              << std::endl
              << " Rg: " << population[0]->getGenotype().Rg
              << std::endl;

}