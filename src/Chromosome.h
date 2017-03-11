//
// Created by marek on 2/2/17.
//

#ifndef BT_CHROMOSOME_H
#define BT_CHROMOSOME_H

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <RInside.h>
#include <random>

#include "Amplifier.h"
#include "Simulator.h"

#define MAX_RESISTANCE 500000
#define MAX_CAPACITY 1000000
#define STANDARD_DEVIATION 500

struct Genotype {
    int32_t R1;
    int32_t R2;
    int32_t Re;
    int32_t Rg;
};

class Chromosome {
    static std::default_random_engine generator;
    static std::normal_distribution<double> N;
    static std::vector<double> referenceVoltage;
    static std::vector<double> referenceTime;
    std::vector<double> voltage;
    std::vector<double> time;

    double objectiveFunctionValue;
    Genotype genotype;

    static Simulator simulator;
    static Amplifier amplifier;
    static RInside R;

    static int32_t mutate(int32_t gene);

    void runSimulation(bool full = false);
    Chromosome(const Genotype & genotype);

public:
    static void init();
    void plot();
    Chromosome();
    double objectiveFunction();
    Chromosome * reproduce();
    static bool compare (Chromosome * chromosome1, Chromosome * chromosome2);
    const Genotype& getGenotype () const;
};

#endif //BT_CHROMOSOME_H