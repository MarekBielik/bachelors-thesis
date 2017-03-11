//
// Created by marek on 2/2/17.
//

#include "Chromosome.h"

std::default_random_engine Chromosome::generator;
std::normal_distribution<double> Chromosome::N(0, STANDARD_DEVIATION);
Simulator Chromosome::simulator;
Amplifier Chromosome::amplifier;
std::vector<double> Chromosome::referenceVoltage;
std::vector<double> Chromosome::referenceTime;
RInside Chromosome::R(0, NULL);

void Chromosome::init() {
    /*set the reference vectors*/
    simulator.simulate(amplifier.getNetlist(), &referenceVoltage,
                       &referenceTime);
    amplifier.freeNetlist();
}

Chromosome::Chromosome() {
    objectiveFunctionValue = std::nan("");
    /*todo: check the RAND_MAX*/
    genotype.R1 = rand() % MAX_RESISTANCE;
    genotype.R2 = rand() % MAX_RESISTANCE;
    genotype.Re = rand() % MAX_RESISTANCE;
    genotype.Rg = rand() % MAX_RESISTANCE;
}

Chromosome::Chromosome(const Genotype & genotype) {
    objectiveFunctionValue = std::nan("");
    int32_t mutation;

    mutation = N(generator);

    this->genotype.R1 = mutate(genotype.R1);
    this->genotype.R2 = mutate(genotype.R2);
    this->genotype.Re = mutate(genotype.Re);
    this->genotype.Rg = mutate(genotype.Rg);
}

int32_t Chromosome::mutate(int32_t gene) {
    const int32_t mutation = N(generator);

    if ((gene + mutation) <= 1 || (gene + mutation) >= MAX_RESISTANCE)
        return gene - mutation;
    else
        return gene + mutation;
}

void Chromosome::runSimulation(bool full /*= false*/) {
    amplifier.setR1(genotype.R1);
    amplifier.setR2(genotype.R2);
    amplifier.setRe(genotype.Re);
    amplifier.setRg(genotype.Rg);

    if (full)
        simulator.simulate(amplifier.getNetlist(), &voltage, &time);
    else
        simulator.simulate(amplifier.getNetlist(), &voltage);

    amplifier.freeNetlist();
}

double Chromosome::objectiveFunction()
{
    /*if the objectiveFunction() has already been called,
     * return the previous value*/
    if(! std::isnan(objectiveFunctionValue))
        return objectiveFunctionValue;

    /*otherwise run the simulation and calculate the objective function*/
    runSimulation();

    /*compare the voltage waveforms*/
    /*using the least squares method*/
    double difference = 0.0;

    /*todo: fix the vector length difference*/
    /*todo: figure out the right interval to compare*/
    for (int i = 100; i < voltage.size() - 10; i++) {
        difference += pow(referenceVoltage[i] - voltage[i], 2);
    }

    return objectiveFunctionValue = difference;
}

Chromosome * Chromosome::reproduce() {
    return new Chromosome(genotype);
}

void Chromosome::plot()
{
    runSimulation(true);

    /*plot a graph of the simulation*/
    R["voltage"] = voltage;
    R["time"] = time;
    std::string cmd;

    /*into a file*/
    cmd = "tmpf = tempfile('plotOut');"
            "png('plotOut.png');"
            "plot(time, voltage, type='l');"
            "dev.off();"
            "tmpf";
    std::string tmpfile = R.parseEval(cmd);
    unlink(tmpfile.c_str());

    /*to the screen*/
    static bool initialized;
    cmd = "x11(); plot(time, voltage, type='l'); Sys.sleep(0);";
    R.parseEvalQ(cmd);
}

bool Chromosome::compare (Chromosome * chromosome1, Chromosome * chromosome2) {
    return chromosome1->objectiveFunction() < chromosome2->objectiveFunction();
}

const Genotype& Chromosome::getGenotype() const {
    return genotype;
}