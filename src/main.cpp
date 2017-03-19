#include <csignal>
#include "main.h"

int main(int argc, char *argv[])
{
    signal(SIGINT, sigIntHandler);

    //Plotter::init("./");

    Population population("plus");
    population.evolve();

    pause();

    return EXIT_SUCCESS;
}

void sigIntHandler(int s) {
    std::cerr << std::endl << "Bye." << std::endl;
    _exit(EXIT_SUCCESS);
}