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
#define COMPONENTS 4
#define SIGMA_INIT 1000

struct Genotype {
    std::vector<Component> components;
    double sigma;
};

class Chromosome {
    static std::random_device rd;
    static std::mt19937 generator;
    static std::normal_distribution<double> N;
    static std::vector<double> referenceVoltage;
    static std::vector<double> referenceTime;
    static Simulator simulator;
    static Amplifier amplifier;
    static RInside R;
    static double tau;

    std::vector<double> voltage;
    std::vector<double> time;
    double objectiveFunctionValue;
    Genotype genotype;

    static Genotype mutate(Genotype genotype);

    void runSimulation(bool full = false);
    Chromosome(const Genotype & genotype);

public:
    static void init();
    void plot();
    Chromosome();
    double objectiveFunction();
    Chromosome reproduce();
    bool operator<(Chromosome & chromosome);
    const Genotype& getGenotype () const;
    friend std::ostream & operator<<(std::ostream & os,
                                     Chromosome & chromosome);
};

#endif //BT_CHROMOSOME_H