//
// Created by marek on 2/19/17.
//

#ifndef BT_POPULATION_H
#define BT_POPULATION_H

#include <vector>
#include <algorithm>
#include <ctime>

#include "Chromosome.h"

/* mu - cardinality of population of ancestors
 * lambda - cardinality of population of descendants
 * */
#define MU 8
#define LAMBDA 16

class Population {
    std::vector<Chromosome>population;

public:
    Population();
    void evolve();
    const Chromosome getResult() const;
};

#endif //BT_POPULATION_H
