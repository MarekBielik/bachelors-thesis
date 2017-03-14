//
// Created by marek on 2/19/17.
//

#include "Population.h"

Population::Population() {
    population.resize(MU + LAMBDA);
    Chromosome::init();

    for (int i = 0; i < MU; i++) {
        population[i] = Chromosome();
    }
}

const Chromosome Population::getResult() const {
    return population[0];
}

void Population::evolve() {
    std::random_device rd;
    std::mt19937 generator(rd());
    unsigned long generationCount = 0;
    clock_t lastTime = 0;
    clock_t now;

    /*todo: fix the terminating condition*/
    while (generationCount < 1000) {
        for (int i = MU; i < LAMBDA + MU; i++)
            population[i] = population[generator() % MU].reproduce();

        sort(population.begin(), population.end());

        if ((++generationCount % 10) == 0) {
            now = clock();
            std::cout << "Generation: " << generationCount << std::endl
                      << population[0]
                      << " time: " << double(now - lastTime) / CLOCKS_PER_SEC
                      << std::endl;
            lastTime = now;
        }
    }
}