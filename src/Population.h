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

#define OBJ_FUN_EPS 0.0001
#define MAX_GEN_COUNT 3000
#define STOP_EPOCH 500
#define PRINT_EPOCH 50

enum ESType {
    plus,
    comma
};

class Population {
    std::vector<Chromosome>population;
    ESType EStype;

public:
    Population(std::string EStype);
    void evolve();
};

#endif //BT_POPULATION_H
