#include <csignal>
#include "main.h"

int main(int argc, char *argv[])
{
    signal(SIGQUIT, sigIntHandler);
    ArgParser argParser;

    /*get the evolution parameters*/
    try {
        argParser.parse(argc, argv);
    } catch (const std::domain_error& e) {
        std::cout << helpMsg;
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        _exit(EXIT_FAILURE);
    }

    /*set the output either to stdout or to a file*/
    std::streambuf * buf;
    std::ofstream of;

    if (argParser.screenOutput()) {
        buf = std::cout.rdbuf();
    } else {
        of.open(argParser.getParams().path + "/EvolutionOutput.txt");
        buf = of.rdbuf();
    }

    std::ostream out(buf);

    Plotter::init(argParser.getParams().path);
    Amplifier::init(argParser.getParams());
    Chromosome::init(argParser.getParams());

    printEvolutionProperties(argParser.getParams(), out);

    Population population(argParser.getParams(), out);

    population.evolve();

    if (argParser.screenOutput()) {
        out << "Evolved." << std::endl;
        pause();
    } else {
        std::cout.precision(5);
        std::cout << std::fixed << population.getResult();
    }

    return EXIT_SUCCESS;
}

void sigIntHandler(int s) {
    std::cerr << std::endl << "Bye." << std::endl;
    _exit(EXIT_SUCCESS);
}

void printEvolutionProperties(Params params, std::ostream & out) {
    const std::string mu = "\u03BC";
    const std::string lambda = "\u03BB";
    const std::string omega = "\u03A9";
    std::string ES_type;

    if (params.ES_type == "plus")
        ES_type = "(" + mu + " + " + lambda + ")";
    else if (params.ES_type == "comma")
        ES_type = "(" + mu + ", " + lambda + ")";
    else
        ES_type = "err: ES_type unspecified";

    out << "Evolution strategies type: " << ES_type << "-ES" <<  std::endl
        << "Parameter " << mu << ": " << params.mu << std::endl
        << "Parameter " << lambda << ": "  << params.lamda << std::endl
        << "Fitness function type: " << params.objFunType << std::endl;

    if (params.objFunType == "idealSine")
        out << "Amplitude: " << params.amplitude << std::endl;

    out << "Maximal number of generations: " << params.max_gen << std::endl
        << "Resistance range: 0 - " << params.max_res << omega << std::endl
        << "Capacity range: 0 - " << params.max_cap << "nF" << std::endl
        << std::endl;
}