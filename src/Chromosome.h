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
#include <random>

#include "Amplifier.h"
#include "Simulator.h"
#include "Plotter.h"

#define MAX_RESISTANCE 500000
#define MAX_CAPACITY 500000
#define COMPONENTS 6
#define SIGMA_INIT 10

enum ObjFunType {
    bestFit,
    min,
    max,
    symAmp
};

struct Genotype {
    std::vector<Component> components;
    std::vector<double> strategyParameters;
};

class Chromosome {
    static std::mt19937 generator;
    static std::normal_distribution<double> N;
    static std::vector<double> referenceOutVoltage;
    static std::vector<double> referenceInVoltage;
    static std::vector<double> referenceTime;
    static Simulator simulator;
    static Amplifier amplifier;
    static Plotter plotter;
    static double TAU;
    static double TAU_PRIME;
    static double MUTATION_MAX;
    static ObjFunType objFunType;

    std::vector<double> voltage;
    std::vector<double> time;
    double objectiveFunctionValue;
    Genotype genotype;

    static Genotype mutate(Genotype genotype);

    void runSimulation(bool full = false);
    void bestFitObjFun();
    void minObjFun();
    void maxObjFun();
    void symAmpObjFun();
    Chromosome(const Genotype & genotype);

public:
    static void init(std::string paramObjFunType = "");
    void plot();
    Chromosome();
    double objectiveFunction();
    Chromosome reproduce();
    bool operator<(Chromosome & chromosome);
    friend std::ostream & operator<<(std::ostream & os,
                                     Chromosome & chromosome);
};

#endif //BT_CHROMOSOME_H