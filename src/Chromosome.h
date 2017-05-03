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
#include "ArgParser.h"

/*forward declarations*/
struct Params;
struct Component;
class Amplifier;

#define MAX_RESISTANCE 500000
#define MAX_CAPACITY 500000
#define SINGLE_STAGE_AMP_COMPONENTS 8
#define TWO_STAGE_AMP_COMPONENTS 14
#define SIGMA_INIT 100
#define DEFAULT_AMPLITUDE 1

enum ObjFunType {
    bestFit,
    idealSin,
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
    static double TAU;
    static double TAU_PRIME;
    static ObjFunType objFunType;
    static int32_t maxRes;
    static int32_t maxCap;
    static unsigned sigmaInit;
    static double amplitude;
    static double maxDiff;
    static bool twoStageAmp;
    static unsigned componentsNumber;

    std::vector<double> voltage;
    std::vector<double> time;
    double objFunVal;
    Genotype genotype;

    /**
     * Creates a new mutated genotype.
     * @param genotype
     * @return
     */
    static Genotype mutate(Genotype genotype);

    /**
     * Runs the simulator and stores its results.
     * @param full if true, the time vector will also be stored, otherwise only
     * the voltage vector will be stored.
     */
    void runSimulation(bool full = false);

    /**
     * Creates a chromosome with the passed genotype.
     * @param genotype
     */
    Chromosome(const Genotype & genotype);

public:
    /**
     * Initializes the class, this method has to be called before the use
     * of the class.
     * @param params contains the initial values
     */
    static void init(Params params);

    /**
     * Plots a graph of the amplifier's output waveform that the chromosome
     * represents.
     */
    void plot();

    /**
     * Creates a new chromosome and initializes its properties with random
     * values.
     */
    Chromosome();

    /**
     * Calculates the fitness of the chromosome.
     * @return fitness of the chromosome
     */
    double objectiveFunction();

    /**
     * Creates a mutated child chromosome.
     * @return newly created chromosome
     */
    Chromosome reproduce();

    bool operator<(Chromosome & chromosome);
    friend std::ostream & operator<<(std::ostream & os,
                                     Chromosome & chromosome);
};

#endif //BT_CHROMOSOME_H