//
// Created by marek on 2/2/17.
//

#include "Chromosome.h"

std::mt19937 Chromosome::generator(std::random_device().operator()());
std::normal_distribution<double> Chromosome::N(0, 1);
Simulator Chromosome::simulator;
Amplifier Chromosome::amplifier;
Plotter Chromosome::plotter;
ObjFunType Chromosome::objFunType;
std::vector<double> Chromosome::referenceOutVoltage;
std::vector<double> Chromosome::referenceInVoltage;
std::vector<double> Chromosome::referenceTime;
/*tau = 1 / sqrt(2 * sqrt(n)) */
double Chromosome::TAU = 1 / sqrt(2 * sqrt(COMPONENTS));
/*tau' = 1 / sqrt(n) */
double Chromosome::TAU_PRIME = 1 / sqrt(COMPONENTS);
double Chromosome::MUTATION_MAX = (MAX_RESISTANCE + MAX_CAPACITY) / 2 / 4;

void Chromosome::init(std::string paramObjFunType /*= ""*/) {
    /*set the reference vectors*/
    simulator.simulate(amplifier.getNetlist(), referenceOutVoltage,
                       referenceTime, referenceInVoltage);
    amplifier.freeNetlist();

    if (paramObjFunType == "bestFit")
        objFunType = bestFit;
    else if (paramObjFunType == "min")
        objFunType = min;
    else if (paramObjFunType == "max")
        objFunType = max;
    else if (paramObjFunType == "symAmp")
        objFunType = symAmp;
    else
        objFunType = bestFit;
}

Chromosome::Chromosome() {
    objFunVal = std::nan("");
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
    objFunVal = std::nan("");
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
        simulator.simulate(amplifier.getNetlist(), voltage, time);
    else
        simulator.simulate(amplifier.getNetlist(), voltage);

    amplifier.freeNetlist();
}

double Chromosome::objectiveFunction()
{
    /*if the objectiveFunction() has already been called,
     * return the previous value*/
    if(! std::isnan(objFunVal))
        return objFunVal;

    runSimulation();

    double inf;
    double sup;

    switch (objFunType) {
        case bestFit:
            /*compare the voltage waveforms*/
            /*using the least squares method*/
            objFunVal = 0.0;

            /*the comparison skips the initial 'unstable area', hence the 30
             * at the start*/
            /*todo: statistics needed to figure out if the starting point has
             *any influence on the overall evolution*/
            for (int i = 30; i < voltage.size(); i++) {
                objFunVal += pow(referenceOutVoltage[i] - voltage[i], 2);
            }
            break;
        case min:
            inf = *std::min_element(std::begin(voltage), std::end(voltage));
            objFunVal = 1 / -inf;
            break;
        case max:
            sup = *std::max_element(std::begin(voltage), std::end(voltage));
            objFunVal = 1 / sup;
            break;
        case symAmp:
            inf = *std::min_element(std::begin(voltage), std::end(voltage));
            sup = *std::max_element(std::begin(voltage), std::end(voltage));

            objFunVal = fabs(sup + inf) + 1 / pow((sup - inf) + 1, 2);
            /*objFunVal = fabs(sup + inf) + 1 / exp(sup - inf + 1);*/
            break;
    }

    return objFunVal;
}

Chromosome Chromosome::reproduce() {
    return Chromosome(genotype);
}

void Chromosome::plot()
{
    runSimulation(true);

    switch (objFunType) {
        case bestFit:
            plotter.plot(referenceTime, referenceOutVoltage, referenceInVoltage,
                         voltage);
            break;
        case min:
        case max:
            plotter.plot(time, referenceInVoltage, voltage);
            break;
        case symAmp:
            plotter.plot(time, referenceOutVoltage, referenceInVoltage,
                         voltage);
            break;
    }
}

bool Chromosome::operator<(Chromosome & chromosome){
    return this->objectiveFunction() < chromosome.objectiveFunction();
}

std::ostream & operator<<(std::ostream & os, Chromosome & chromosome) {
    os << "objective function: " << chromosome.objectiveFunction() << std::endl;

    for (int i = 0; i < COMPONENTS; i++) {
        switch (chromosome.genotype.components[i].name) {
            case R1:
                os << "R1: ";
                break;
            case R2:
                os << "R2: ";
                break;
            case Re:
                os << "Re: ";
                break;
            case Rg:
                os << "Rg: ";
                break;
            case Cin:
                os << "Cin: ";
                break;
            case Ce:
                os << "Ce: ";
                break;
        }

        double value = (double) chromosome.genotype.components[i].value;
        std::ios::fmtflags flags(os.flags());
        std::streamsize ss = os.precision();
        os << std::fixed;

        /*round the printed value*/
        if (value < 999)
            os << std::setprecision(0) << value;
        else if (value > 999 && value < 9999) {
            os << std::setprecision(2) << value / 1000;
        } else if (value > 9999 && value < 99999) {
            os << std::setprecision(1) << value / 1000;
        } else if (value > 99999) {
            os << std::setprecision(0) << value / 1000;
        }

        os << std::setprecision(ss);
        os.flags(flags);

        switch (chromosome.genotype.components[i].type) {
            case resistor:
                if (value > 999)
                    os << " K";
                else
                    os << " R";
                break;
            case capacitor:
                if (value > 999)
                    os << " uF";
                else
                    os << " nF";
                break;
        }

        os << ", sigma: " << chromosome.genotype.strategyParameters[i]
           << std::endl;
    }

    return os;
}
