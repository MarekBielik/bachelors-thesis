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

#define OBJ_FUN_EPS 0.001
#define MAX_GEN_COUNT 3000
#define OBJ_FUN_MIN 0.001
#define STOP_EPOCH 200
#define PRINT_EPOCH 50

class Population {
    std::vector<Chromosome>population;

public:
    Population();
    void evolve();
    const Chromosome getResult() const;
};

#endif //BT_POPULATION_H
