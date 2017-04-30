//
// Created by marek on 2/2/17.
//

#include "Chromosome.h"

std::mt19937 Chromosome::generator(std::random_device().operator()());
std::normal_distribution<double> Chromosome::N(0, 1);
Simulator Chromosome::simulator;
ObjFunType Chromosome::objFunType;
std::vector<double> Chromosome::referenceOutVoltage;
std::vector<double> Chromosome::referenceInVoltage;
std::vector<double> Chromosome::referenceTime;
double Chromosome::TAU;
double Chromosome::TAU_PRIME;
int32_t Chromosome::maxRes;
int32_t Chromosome::maxCap;
unsigned Chromosome::sigmaInit;
double Chromosome::amplitude;
double Chromosome::maxDiff;
bool Chromosome::twoStageAmp;
unsigned Chromosome::componentsNumber;

void Chromosome::init(Params params) {
    Amplifier::getInstance().setRload(params.Rload);

    /*set the reference vectors*/
    simulator.simulate(Amplifier::getInstance().getNetlist(),
                       referenceOutVoltage, referenceTime, referenceInVoltage);
    Amplifier::getInstance().freeNetlist();

    if (params.objFunType == "bestMatch")
        objFunType = bestFit;
    else if (params.objFunType == "idealSine")
        objFunType = idealSin;
    else if (params.objFunType == "maxAmp")
        objFunType = symAmp;
    else
        objFunType = bestFit;

    sigmaInit = params.sigma_init;
    maxRes = params.max_res;
    maxCap = params.max_cap;
    amplitude = params.amplitude;
    maxDiff = params.max_diff;
    twoStageAmp = params.two_stage_amp;

    if(twoStageAmp)
        componentsNumber = TWO_STAGE_AMP_COMPONENTS;
    else
        componentsNumber = SINGLE_STAGE_AMP_COMPONENTS;

    /*tau = 1 / sqrt(2 * sqrt(n)) */
    TAU = 1 / sqrt(2 * sqrt(componentsNumber));
    /*tau' = 1 / sqrt(n) */
    TAU_PRIME = 1 / sqrt(componentsNumber);
}

Chromosome::Chromosome() {
    objFunVal = std::nan("");
    genotype.components.resize(componentsNumber);
    genotype.strategyParameters.resize(componentsNumber);

    if (!twoStageAmp) {
        genotype.components[0].name = R1;
        genotype.components[1].name = R2;
        genotype.components[2].name = Re;
        genotype.components[3].name = Rg;
        genotype.components[4].name = Rc;
        genotype.components[5].name = Ce;
        genotype.components[6].name = Cin;
        genotype.components[7].name = Cout;
    } else {
        genotype.components[0].name = R1;
        genotype.components[1].name = R2;
        genotype.components[2].name = Re;
        genotype.components[3].name = Rc;
        genotype.components[4].name = Ce;
        genotype.components[5].name = Cin;
        genotype.components[6].name = Cout;
        genotype.components[7].name = Rgb;
        genotype.components[8].name = Reb;
        genotype.components[9].name = Rcb;
        genotype.components[10].name = R2b;
        genotype.components[11].name = R1b;
        genotype.components[12].name = Cm;
        genotype.components[13].name = Ce2;
    }

    for (int i = 0; i < componentsNumber; i++) {
        if (genotype.components[i].name <= R1b) {
            genotype.components[i].type = resistor;
            genotype.components[i].value = generator() % maxRes;
        } else {
            genotype.components[i].type = capacitor;
            genotype.components[i].value = generator() % maxCap;
        }

        genotype.strategyParameters[i] = sigmaInit;
    }
}

Chromosome::Chromosome(const Genotype & genotype) {
    objFunVal = std::nan("");
    this->genotype = mutate(genotype);
}

Genotype Chromosome::mutate(Genotype genotype) {
    int32_t mutation,
            supremum;
    double mutationMax;

    for (int i = 0; i < componentsNumber; i++) {
        /*choose the appropriate supremum and max mutation step*/
        switch (genotype.components[i].type) {
            case resistor:
                mutationMax = maxRes / 2;
                supremum = maxRes;
                break;
            case capacitor:
                supremum = maxCap;
                mutationMax = maxCap / 2;
                break;
            default:
                mutationMax = supremum = 0;
                break;
        }

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
        Amplifier::getInstance().setComponentValue(component);
    }

    if (full)
        simulator.simulate(Amplifier::getInstance().getNetlist(), voltage, time);
    else
        simulator.simulate(Amplifier::getInstance().getNetlist(), voltage);

    Amplifier::getInstance().freeNetlist();
}

double Chromosome::objectiveFunction() {
    /*if the objectiveFunction() has already been called,
     * return the previous value*/
    if(! std::isnan(objFunVal))
        return objFunVal;

    runSimulation();

    int refSineSize;
    double refSine,
            peak,
            trough,
            min,
            max;
    /*the values of the start and the end depend on the length of the voltage
     * vector obtained from the simulator*/
    int start = 5;
    int end = 68;
    const double twoPi = std::acos(-1) * 2;

    while (voltage[start] < 0) start++;
    while (voltage[start] > 0) start++;
    while (voltage[end] < 0) end--;

    /* in case the signal is too noisy*/
    if (start > 50 || end < 20 || start + 12 >= end)
        return objFunVal = DBL_MAX;

    trough = *std::min_element(std::begin(voltage) + start,
                               std::begin(voltage) + start + 12);
    peak = *std::max_element(std::begin(voltage) + start + 12,
                             std::begin(voltage) + end);

    min = std::min(fabs(trough), peak);
    max = std::max(fabs(trough), peak);

    if ((min / max) < maxDiff)
        return objFunVal = DBL_MAX;

    switch (objFunType) {
        case bestFit:
            objFunVal = 0.0;
            for (int i = start; i < end; i++) {
                objFunVal += pow(referenceOutVoltage[i] - voltage[i], 2);
            }
            break;
        case idealSin:
            objFunVal = 0.0;
            refSineSize = end - start;

            for (int i = 0; i < refSineSize; i++) {
                refSine = -amplitude * sin((twoPi * i) / (refSineSize -1));
                objFunVal += pow(refSine - voltage[start+i], 2);
            }
            break;
        case symAmp:
            //objFunVal = (fabs(peak + trough) + 1)  / (peak - trough);
            //objFunVal = fabs(peak + trough) + 1 / exp(peak - trough + 1);
            objFunVal = 1 / (peak - trough);
            break;
    }

    return objFunVal;
}

Chromosome Chromosome::reproduce() {
    return Chromosome(genotype);
}

void Chromosome::plot() {
    runSimulation(true);

    switch (objFunType) {
        case bestFit:
            Plotter::getInstance().plot(referenceTime, referenceOutVoltage, referenceInVoltage,
                         voltage);
            break;
        case idealSin:
        case symAmp:
            Plotter::getInstance().plot(time, voltage);
            break;
    }
}

bool Chromosome::operator<(Chromosome & chromosome){
    return this->objectiveFunction() < chromosome.objectiveFunction();
}

std::ostream & operator<<(std::ostream & os, Chromosome & chromosome) {
    os << "objective function: " << chromosome.objectiveFunction() << std::endl;

    for (int i = 0; i < Chromosome::componentsNumber; i++) {
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
            case Rc:
                os << "Rc: ";
                break;
            case Cin:
                os << "Cin: ";
                break;
            case Cout:
                os << "Cout: ";
                break;
            case Ce:
                os << "Ce: ";
                break;
            case Cm:
                os << "Cm: ";
                break;
            case Ce2:
                os << "Ce2: ";
                break;
            case Rgb:
                os << "Rgb: ";
                break;
            case Reb:
                os << "Reb: ";
                break;
            case Rcb:
                os << "Rcb: ";
                break;
            case R2b:
                os << "R2b: ";
                break;
            case R1b:
                os << "R1b: ";
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
