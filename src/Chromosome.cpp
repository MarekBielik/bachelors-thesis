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
int32_t Chromosome::maxRes;
int32_t Chromosome::maxCap;
unsigned Chromosome::sigmaInit;
double Chromosome::mutationMax;
double Chromosome::amplitude;

void Chromosome::init(Params params) {
    /*set the reference vectors*/
    simulator.simulate(amplifier.getNetlist(), referenceOutVoltage,
                       referenceTime, referenceInVoltage);
    amplifier.freeNetlist();

    if (params.objFunType == "bestFit")
        objFunType = bestFit;
    else if (params.objFunType == "idealSin")
        objFunType = idealSin;
    else if (params.objFunType == "symAmp")
        objFunType = symAmp;
    else
        objFunType = bestFit;

    sigmaInit = params.sigma_init;
    maxRes = params.max_res;
    maxCap = params.max_cap;
    amplitude = params.amplitude;
    /*todo: fix the expression*/
    mutationMax = (maxRes + maxCap) / 2 / 4;
}

Chromosome::Chromosome() {
    objFunVal = std::nan("");
    genotype.components.resize(COMPONENTS);
    genotype.strategyParameters.resize(COMPONENTS);
    Component tmpComp;

    for (int i = 0; i < COMPONENTS; i++) {
        genotype.strategyParameters[i] = sigmaInit;

        switch (i) {
            case 0:
                tmpComp.name = R1;
                tmpComp.type = resistor;
                tmpComp.value = generator() % maxRes;
                break;
            case 1:
                tmpComp.name = R2;
                tmpComp.type = resistor;
                tmpComp.value = generator() % maxRes;
                break;
            case 2:
                tmpComp.name = Re;
                tmpComp.type = resistor;
                tmpComp.value = generator() % maxRes;
                break;
            case 3:
                tmpComp.name = Rg;
                tmpComp.type = resistor;
                tmpComp.value = generator() % maxRes;
                break;
            case 4:
                tmpComp.name = Ce;
                tmpComp.type = capacitor;
                tmpComp.value = generator() % maxCap;
                break;
            case 5:
                tmpComp.name = Cin;
                tmpComp.type = capacitor;
                tmpComp.value = generator() % maxCap;
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
        } while (mutation > mutationMax ||
                 mutation < -mutationMax ||
                 genotype.strategyParameters[i] > mutationMax ||
                 genotype.strategyParameters[i] < -mutationMax);

        /*choose the appropriate supremum*/
        switch (genotype.components[i].type) {
            case resistor:
                supremum = maxRes;
                break;
            case capacitor:
                supremum = maxRes;
                break;
            default:
                supremum = maxRes;
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

    double low = *std::min_element(std::begin(voltage), std::end(voltage));
    double high = *std::max_element(std::begin(voltage), std::end(voltage));

    int refSineSize;
    double refSine;
    int start = 5;
    int end = 68;
    const double twoPi = std::acos(-1) * 2;

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
            /*todo: add the symmetry condition*/
            break;
        case idealSin:
            objFunVal = 0;
            while (voltage[start] < 0) start++;
            while (voltage[start] > 0) start++;
            while (voltage[end] < 0) end--;

            refSineSize = end - start;

            for (int i = 0; i < refSineSize; i++) {
                refSine = -amplitude * sin((twoPi * i) / (refSineSize -1));
                objFunVal += pow(refSine - voltage[start+i], 2);
            }

            objFunVal = (fabs(high + low) + 1) * objFunVal;

            break;
        case symAmp:
            objFunVal = (fabs(high + low) + 1) / (high - low + 1);
            /*objFunVal = fabs(high + low) + 1 / exp(high - low + 1);*/
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
        case idealSin:
        case symAmp:
            plotter.plot(time, referenceInVoltage, voltage);
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
