//
// Created by marek on 2/19/17.
//

#ifndef BT_POPULATION_H
#define BT_POPULATION_H

#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>

#include "Chromosome.h"
#include "ArgParser.h"

/*forward declaration*/
struct Params;
class Chromosome;

/* mu - cardinality of the population of ancestors
 * lambda - cardinality of the population of descendants
 * */
#define MU 10
#define LAMBDA 150

#define MAX_GEN 3000
#define STOP_GEN 500
#define PRINT_GEN 10
#define PRINT_CHANGE 0.9
#define STOP_CHANGE 0.99

enum ES_Type {
    plus,
    comma
};

class Population {
    std::vector<Chromosome>population;
    ES_Type ES_type;
    unsigned mu;
    unsigned lambda;
    unsigned maxGen;
    unsigned stopGen;
    unsigned printGen;
    double printChange;
    double stopChange;
    std::ostream & out;

public:
    /**
     * Returns an instance of Population.
     * @param params contains the initial values
     * @param out the description of the simulation will be redirected to
     * this stream
     */
    Population(Params params, std::ostream & out);

    /**
     * Returns the fitness value of the resulting chromosome in the population.
     * @return fitness of the resulting chromosome
     */
    double getResult();

    /**
     * Triggers the evolution.
     */
    void evolve();
};

#endif //BT_POPULATION_H
