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
    bool printCourse = true;

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

        /*print the current evolution state*/
        if (!(generationCount % printGen) &&
                prevPrintGen > population[0].objectiveFunction()) {
            prevPrintGen = population[0].objectiveFunction() * printChange;

            /*print the population state*/
            out << "Generation: " << generationCount << std::endl
                << population[0] << std::endl;
            population[0].plot();
        }

        /*stop the evolution if the objective function doesn't change anymore*/
        if (!(generationCount % stopGen)) {

            if (!(prevStopGen > population[0].objectiveFunction())) {
                /*if we encounter DBL_MAX in the objective function,
                 * don't end the evolution now, but try to end it
                 * in the very next generation*/
                if (population[0].objectiveFunction() > 5000) {
                    continue;
                }
                else
                    break;
            }
            prevStopGen = population[0].objectiveFunction() * stopChange;
        }

        /*track the evolution course*/
        bestChromosomes.push_back(population[0].objectiveFunction());
        worstChromosomes.push_back(population[mu].objectiveFunction());
        generations.push_back(generationCount);
        double averageObjectiveFunction = 0;
        unsigned valid_chromosomes = 0;

        for (auto & chromosome: population) {
            if(chromosome.objectiveFunction() < 5000) {
                valid_chromosomes++;
                averageObjectiveFunction += chromosome.objectiveFunction();
            }
        }

        if (population[0].objectiveFunction() > 5000 ||
            population[mu].objectiveFunction() > 5000) {
            printCourse = false;
        }

        averageChromosomes.push_back(averageObjectiveFunction /
                                             valid_chromosomes);

    }

    if (printCourse && !averageChromosomes.empty()) {
        Plotter::getInstance().plotEvolutionCourse(bestChromosomes,
                                                   averageChromosomes,
                                                   worstChromosomes,
                                                   generations);
    }
}

double Population::getResult() {
    return population[0].objectiveFunction();
}