//
// Created by marek on 2/2/17.
//

#include "Chromosome.h"

std::random_device Chromosome::rd;
std::mt19937 Chromosome::generator(rd());
std::normal_distribution<double> Chromosome::N(0, 1);
Simulator Chromosome::simulator;
Amplifier Chromosome::amplifier;
std::vector<double> Chromosome::referenceOutVoltage;
std::vector<double> Chromosome::referenceInVoltage;
std::vector<double> Chromosome::referenceTime;
RInside Chromosome::R(0, NULL);
/*tau = 1 / sqrt(2 * sqrt(n)) */
double Chromosome::TAU = 1 / sqrt(2 * sqrt(COMPONENTS));
/*tau' = 1 / sqrt(n) */
double Chromosome::TAU_PRIME = 1 / sqrt(COMPONENTS);
double Chromosome::MUTATION_MAX = (MAX_RESISTANCE + MAX_CAPACITY) / 2 / 4;

void Chromosome::init() {
    /*set the reference vectors*/
    simulator.simulate(amplifier.getNetlist(), &referenceOutVoltage,
                       &referenceInVoltage, &referenceTime);
    amplifier.freeNetlist();
}

Chromosome::Chromosome() {
    objectiveFunctionValue = std::nan("");
    genotype.components.resize(COMPONENTS);
    genotype.strategyParameters.resize(COMPONENTS);
    Component tmpComp;

    for (int i = 0; i < COMPONENTS; i++) {
        genotype.strategyParameters[i] = SIGMA_INIT;

        switch (i) {
            case 0:
                tmpComp.name = R1;
                tmpComp.type = resistor;
                tmpComp.value = generator() % MAX_RESISTANCE;
                break;
            case 1:
                tmpComp.name = R2;
                tmpComp.type = resistor;
                tmpComp.value = generator() % MAX_RESISTANCE;
                break;
            case 2:
                tmpComp.name = Re;
                tmpComp.type = resistor;
                tmpComp.value = generator() % MAX_RESISTANCE;
                break;
            case 3:
                tmpComp.name = Rg;
                tmpComp.type = resistor;
                tmpComp.value = generator() % MAX_RESISTANCE;
                break;
            case 4:
                tmpComp.name = Ce;
                tmpComp.type = capacitor;
                tmpComp.value = generator() % MAX_CAPACITY;
                break;
            case 5:
                tmpComp.name = Cin;
                tmpComp.type = capacitor;
                tmpComp.value = generator() % MAX_CAPACITY;
                break;
        }

        genotype.components[i] = tmpComp;
    }
}

Chromosome::Chromosome(const Genotype & genotype) {
    objectiveFunctionValue = std::nan("");
    this->genotype = mutate(genotype);
}

Genotype Chromosome::mutate(Genotype genotype) {
    int32_t mutation, supremum;

    for (int i = 0; i < COMPONENTS; i++) {
        double prevSigma = genotype.strategyParameters[i];
        /* get the mutation*/
        /* sigma(t+1) = sigma(t) * e^(TAU' * N(0,1) + TAU * N(0,1)) */
        do {
            genotype.strategyParameters[i] = prevSigma *
                    exp(TAU_PRIME * N(generator) + TAU * N(generator));
            mutation = genotype.strategyParameters[i] * N(generator);
            /*keep the mutation and strategy parameter in the valid interval*/
        } while (mutation > MUTATION_MAX ||
                 mutation < -MUTATION_MAX ||
                 genotype.strategyParameters[i] > MUTATION_MAX ||
                 genotype.strategyParameters[i] < -MUTATION_MAX);

        /*choose the appropriate supremum*/
        switch (genotype.components[i].type) {
            case resistor:
                supremum = MAX_RESISTANCE;
                break;
            case capacitor:
                supremum = MAX_CAPACITY;
                break;
            default:
                supremum = MAX_RESISTANCE;
                break;
        }

        /*mutate the component's value and keep it in the valid interval*/
        if ((genotype.components[i].value + mutation) <= 1 ||
            (genotype.components[i].value + mutation) >= supremum)
            genotype.components[i].value -= mutation;
        else
            genotype.components[i].value += mutation;
    }

    return genotype;
}

void Chromosome::runSimulation(bool full /*= false*/) {
    for(auto & component: genotype.components) {
        amplifier.setComponentValue(component);
    }

    if (full)
        simulator.simulate(amplifier.getNetlist(), &voltage, NULL, &time);
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

    /*the comparison skips the initial 'unstable area', hence the 30
     * at the start*/
    /*the simulator doesn't always return a vector of a constant length,
     * hence the '- 2' at the end*/
    for (int i = 0; i < voltage.size(); i++) {
        difference += pow(referenceOutVoltage[i] - voltage[i], 2);
    }

    return objectiveFunctionValue = difference;
}

Chromosome Chromosome::reproduce() {
    return Chromosome(genotype);
}

/*todo: move the code to the plotter class*/
void Chromosome::plot()
{
    runSimulation(true);

    /*plot a graph of the simulation*/
    R["voltageOut"] = voltage;
    R["voltageIn"] = referenceInVoltage;
    R["refVoltageOut"] = referenceOutVoltage;
    R["time"] = time;
    std::string cmd;

    /*into a file*/
    cmd = "tmpf = tempfile('plotOut');"
            "png('plotOut.png');"
            "plot(time, refVoltageOut, type='l', col='red', ylab='Voltage [V]',"
            "     xlab='Time [s]', lwd=2, ylim=c(-2.5, 2.5));"
            "lines(time, voltageOut, col='green', lwd=2);"
            "lines(time, voltageIn, col='blue', lwd=2);"
            "grid();"
            "legend('topleft', inset=c(0.25,-0.17), xpd=TRUE,"
            "       legend=c('Analytical solution output',"
            "                'Evolved solution output',"
            "                'input'),"
            "lty=c(1,1,1), lwd=c(2,2,2), col=c('red', 'green', 'blue'));"
            "dev.off();"
            "tmpf";
    std::string tmpfile = R.parseEval(cmd);
    unlink(tmpfile.c_str());

    /*to the screen*/
    cmd = "x11();"
            "plot(time, refVoltageOut, type='l', col='red', ylab='Voltage [V]',"
            "     xlab='Time [s]', lwd=2, ylim=c(-2.5, 2.5));"
            "lines(time, voltageOut, col='green', lwd=2);"
            "lines(time, voltageIn, col='blue', lwd=2);"
            "grid();"
            "legend('topleft', inset=c(0.25,-0.17), xpd=TRUE,"
            "       legend=c('Analytical solution output',"
            "                'Evolved solution output',"
            "                'input'),"
            "lty=c(1,1,1), lwd=c(2,2,2), col=c('red', 'green', 'blue'));"
            "Sys.sleep(0);";
    R.parseEvalQ(cmd);
}

bool Chromosome::operator<(Chromosome & chromosome){
    return this->objectiveFunction() < chromosome.objectiveFunction();
}

/*todo: implement rounding of the values*/
std::ostream & operator<<(std::ostream & os, Chromosome & chromosome) {
    os << "objective function: " << chromosome.objectiveFunction() << std::endl;

    for (int i = 0; i < COMPONENTS; i++) {
        switch (chromosome.genotype.components[i].name) {
            case R1:
                os << "R1: " << chromosome.genotype.components[i].value << "R"
                   << " sigma: " << chromosome.genotype.strategyParameters[i]
                   << std::endl;
                break;
            case R2:
                os << "R2: " << chromosome.genotype.components[i].value << "R"
                   << " sigma: " << chromosome.genotype.strategyParameters[i]
                   << std::endl;
                break;
            case Re:
                os << "Re: " << chromosome.genotype.components[i].value << "R"
                   << " sigma: " << chromosome.genotype.strategyParameters[i]
                   << std::endl;
                break;
            case Rg:
                os << "Rg: " << chromosome.genotype.components[i].value << "R"
                   << " sigma: " << chromosome.genotype.strategyParameters[i]
                   << std::endl;
                break;
            case Cin:
                os << "Cin: " << chromosome.genotype.components[i].value << "nF"
                   << " sigma: " << chromosome.genotype.strategyParameters[i]
                   << std::endl;
                break;
            case Ce:
                os << "Ce: " << chromosome.genotype.components[i].value << "nF"
                   << " sigma: " << chromosome.genotype.strategyParameters[i]
                   << std::endl;
                break;
        }
    }
    return os;
}

const Genotype& Chromosome::getGenotype() const {
    return genotype;
}
