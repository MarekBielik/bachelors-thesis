//
// Created by marek on 2/19/17.
//

#include "Population.h"

Population::Population(Params params, std::ostream & out) : out(out) {
    /*init parameters*/
    if (params.ES_type == "comma")
        this->ES_type = comma;
    else
        this->ES_type = plus;

    mu = params.mu;
    lambda = params.lamda;
    maxGen = params.max_gen;
    stopGen = params.stop_gen;
    printGen = params.print_gen;
    printChange = params.print_change;
    stopChange = params.stop_change;

    /*init the population*/
    population.resize(mu + lambda);

    for (int i = 0; i < mu; i++) {
        population[i] = Chromosome();
    }
}

void Population::evolve() {
    std::mt19937 generator(std::random_device().operator()());
    unsigned generationCount = 0;
    double prevStopGen = DBL_MAX;
    double prevPrintGen = DBL_MAX;
    std::vector<double> bestChromosomes;
    std::vector<double> worstChromosomes;
    std::vector<double> averageChromosomes;
    std::vector<unsigned> generations;

    while (++generationCount < maxGen) {
        for (int i = mu; i < lambda + mu; i++)
            population[i] = population[generator() % mu].reproduce();

        switch (ES_type) {
            case plus:
                sort(population.begin(), population.end());
                break;
            case comma:
                sort(population.begin() + mu, population.end());
                for (int i = 0; i < mu; i++) {
                    population[i] = population[i+mu];
                }
                break;
            default:
                sort(population.begin(), population.end());
                break;
        }

        if (!(generationCount % printGen) &&
                prevPrintGen > population[0].objectiveFunction()) {
            prevPrintGen = population[0].objectiveFunction() * printChange;

            /*print the population state*/
            out << "Generation: " << generationCount << std::endl
                << population[0] << std::endl;
            population[0].plot();

            /*track the evolution course*/
            bestChromosomes.push_back(population.front().objectiveFunction());
            worstChromosomes.push_back(population.back().objectiveFunction());
            generations.push_back(generationCount);
            double averageObjectiveFunction = 0;

            for (auto & chromosome: population)
                averageObjectiveFunction += chromosome.objectiveFunction();

            averageChromosomes.push_back(averageObjectiveFunction / (mu + lambda));
        }

        /*stop the evolution if the objective function doesn't change anymore*/
        if (!(generationCount % stopGen)) {
            if (!(prevStopGen > population[0].objectiveFunction())) {
                break;
            }
            prevStopGen = population[0].objectiveFunction() * stopChange;
        }

    }

    Plotter::getInstance().plotEvolutionCourse(bestChromosomes,
                                               averageChromosomes,
                                               worstChromosomes,
                                               generations);
}
