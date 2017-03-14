//
// Created by marek on 2/2/17.
//

#include "Chromosome.h"

std::random_device Chromosome::rd;
std::mt19937 Chromosome::generator(rd());
std::normal_distribution<double> Chromosome::N(0, 1);
Simulator Chromosome::simulator;
Amplifier Chromosome::amplifier;
std::vector<double> Chromosome::referenceVoltage;
std::vector<double> Chromosome::referenceTime;
RInside Chromosome::R(0, NULL);
double Chromosome::tau = 1 / sqrt(COMPONENTS);

void Chromosome::init() {
    /*set the reference vectors*/
    simulator.simulate(amplifier.getNetlist(), &referenceVoltage,
                       &referenceTime);
    amplifier.freeNetlist();
}

Chromosome::Chromosome() {
    objectiveFunctionValue = std::nan("");
    genotype.components.resize(COMPONENTS);
    genotype.sigma = SIGMA_INIT;
    Component tmpComp;

    tmpComp.name = R1;
    tmpComp.type = resistor;
    tmpComp.value = generator() % MAX_RESISTANCE;
    genotype.components[0] = tmpComp;

    tmpComp.name = R2;
    tmpComp.type = resistor;
    tmpComp.value = generator() % MAX_RESISTANCE;
    genotype.components[1] = tmpComp;

    tmpComp.name = Re;
    tmpComp.type = resistor;
    tmpComp.value = generator() % MAX_RESISTANCE;
    genotype.components[2] = tmpComp;

    tmpComp.name = Rg;
    tmpComp.type = resistor;
    tmpComp.value = generator() % MAX_RESISTANCE;
    genotype.components[3] = tmpComp;
}

Chromosome::Chromosome(const Genotype & genotype) {
    objectiveFunctionValue = std::nan("");
    this->genotype = mutate(genotype);
}

Genotype Chromosome::mutate(Genotype genotype) {
    /*sigma(t+1) = sigma(t) * e^(tau * N(0,1))*/
    /*todo: fix the sigma interval*/
    genotype.sigma = genotype.sigma * exp(tau * N(generator));
    const int32_t mutation = genotype.sigma * N(generator);
    int32_t supremum;

    for (auto & component: genotype.components) {
        if (component.type == resistor)
            supremum = MAX_RESISTANCE;
        else
            supremum = MAX_CAPACITY;

        /*mutate the component's value and keep it in the valid interval*/
        if ((component.value + mutation) <= 1 ||
            (component.value + mutation) >= supremum)
            component.value -= mutation;
        else
            component.value += mutation;
    }

    return genotype;
}

void Chromosome::runSimulation(bool full /*= false*/) {
    for(auto & component: genotype.components) {
        amplifier.setComponentValue(component);
    }

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

Chromosome Chromosome::reproduce() {
    return Chromosome(genotype);
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

bool Chromosome::operator<(Chromosome & chromosome){
    return this->objectiveFunction() < chromosome.objectiveFunction();
}

const Genotype& Chromosome::getGenotype() const {
    return genotype;
}

std::ostream & operator<<(std::ostream & os, Chromosome & chromosome) {
    os << "objective function: " << chromosome.objectiveFunction() << std::endl
       << "sigma: " << chromosome.genotype.sigma << std::endl;
    return os;
}