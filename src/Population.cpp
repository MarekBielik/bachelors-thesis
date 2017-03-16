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
    unsigned int generationCount = 0;
    double prevGeneration = DBL_MAX;
    clock_t lastTime = 0;
    clock_t now;

    while (++generationCount < MAX_GEN_COUNT ||
            population[0].objectiveFunction() > OBJ_FUN_MIN) {
        for (int i = MU; i < LAMBDA + MU; i++)
            population[i] = population[generator() % MU].reproduce();

        sort(population.begin(), population.end());

//        for (int i = 0; i < MU; i++) {
//            population[i] = population[i+MU];
//        }

        /*todo: overload the << operator for cout*/
        if (!(generationCount % PRINT_EPOCH)) {
            now = clock();
            std::cout << "Generation: " << generationCount << std::endl
                      << population[0]
                      << " time: " << double(now - lastTime) / CLOCKS_PER_SEC
                      << std::endl;
            lastTime = now;

            population[0].plot();
        }

        /*stop the evolution if the objective function doesn't change anymore*/
        if (!(generationCount % STOP_EPOCH)) {
            if ((prevGeneration - population[0].objectiveFunction()) <
                OBJ_FUN_EPS)
                break;
            prevGeneration = population[0].objectiveFunction();
        }
    }

}