#include "main.h"

int main(int argc, char *argv[])
{
    /*initialization*/
    srand (time(NULL));

    Population population;
    population.evolve();

    return EXIT_SUCCESS;
}

