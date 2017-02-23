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

void Chromosome::init() {
    //N(0, STANDARD_DEVIATION);

    /*set the reference vectors*/
    simulator.simulate(amplifier.getNetlist(), &referenceVoltage,
                       &referenceTime);
    amplifier.freeNetlist();
}

Chromosome::Chromosome() {
    objectiveFunctionValue = std::nan("");
    /*todo: check the RAND_MAX*/
    genotype.R1 = rand() % MAX_RESISTANCE;
}

Chromosome::Chromosome(const Genotype & genotype) {
    objectiveFunctionValue = std::nan("");
    this->genotype.R1 = genotype.R1 + N(generator);
}

void Chromosome::runSimulation(bool full /*= false*/) {
    amplifier.setR1(genotype.R1);

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
    for (int i = 0; i < voltage.size() - 10; i++) {
        difference += pow(referenceVoltage[i] - voltage[i], 2);
    }

    return objectiveFunctionValue = difference;
}

Chromosome * Chromosome::reproduce() {
    return new Chromosome(genotype);
}

void Chromosome::plot()
{
    /*run through the voltage vector and print out the values*/
    for (size_t i = 0; i < voltage.size(); i++) {
        std::cout << i << ". ";
        std::cout << voltage[i] << " - ";
        std::cout << time[i] << std::endl;
    }

    /*plot a graph of the simulation*/
    RInside R(0, NULL);

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
    cmd = "x11(); plot(time, voltage, type='l'); Sys.sleep(5);";
    R.parseEvalQ(cmd);
}

bool Chromosome::compare (Chromosome * chromosome1, Chromosome * chromosome2) {
    return chromosome1->objectiveFunction() < chromosome2->objectiveFunction();
}

const Genotype& Chromosome::getGenotype() const {
    return genotype;
}