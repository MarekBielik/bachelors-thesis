#include <csignal>
#include "main.h"

int main(int argc, char *argv[])
{
    signal(SIGQUIT, sigIntHandler);
    ArgParser argParser;

    /*get the evolution parameters*/
    try {
        argParser.parse(argc, argv);
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
        of.open(argParser.getParams().path + "/EvoutionOutput.txt");
        buf = of.rdbuf();
    }

    std::ostream out(buf);

    Plotter::init(argParser.getParams().path);
    Chromosome::init(argParser.getParams());

    Population population(argParser.getParams(), out);
    population.evolve();

    if (argParser.screenOutput()) {
        out << "Evolved." << std::endl;
        pause();
    }

    return EXIT_SUCCESS;
}

void sigIntHandler(int s) {
    std::cerr << std::endl << "Bye." << std::endl;
    _exit(EXIT_SUCCESS);
}