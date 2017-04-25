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

/* mu - cardinality of population of ancestors
 * lambda - cardinality of population of descendants
 * */
#define MU 8
#define LAMBDA 16

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
    Population(Params params, std::ostream & out);
    double getResult();
    void evolve();
};

#endif //BT_POPULATION_H
