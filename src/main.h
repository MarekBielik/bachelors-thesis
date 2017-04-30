//
// Created by marek on 11/22/16.
//

#ifndef BT_MAIN_H
#define BT_MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Population.h"
#include "ArgParser.h"

void sigIntHandler(int s);
void printEvolutionProperties(Params params, std::ostream & out);

const std::string helpMsg = ""
        "DESCRIPTION\n"
        "   bt is a tool for automating the process of designing analog \n"
        "   circuits. In the current version, it supports two embedded analog\n"
        "   amplifiers and it uses the evolution strategies technique to \n"
        "   automate the design.\n\n"

        "SYNOPSIS:\n"
        "   bt [OPTION]...\n\n"

        "OPTIONS:\n"
        "-h, --help\n"
        "   Prints the help message.\n\n"

        "-o <directory_name>\n"
        "   Specifies the output directory.\n"
        "   By default, stdout is used for the text output and the graphs are\n"
        "   displayed on the screen.\n\n"

        "--mu <number_of_ancestors>\n"
        "   Sets the cardinality of the population of ancestors.\n"
        "   The default value is 10 chromosomes.\n\n"

        "--lambda <number_of_descendants>\n"
        "   Sets the cardinality of the population of descendants.\n"
        "   The default value is 150 chromosomes.\n\n"

        "--max-gen <number_of_generations>\n"
        "   Sets the maximum number of generations in the evolution.\n"
        "   The default value is 3000 generations.\n\n"

        "--stop-gen <number_of_generations>\n"
        "   Sets the number of generations after which the terminating\n"
        "   condition will be checked. This option is related to the\n"
        "   \"stop-change\" option.\n"
        "   The default value is 500 generations.\n\n"

        "--stop-change <value>\n"
        "   The evolution terminates when the fitness of the best chromosome\n"
        "   in the population does not decrease by a certain percentage after\n"
        "   a certain number of generations. The percentage is set by this\n"
        "   option. The value is in the interval ]0,1].\n"
        "   The default value is 0.99 meaning that the evolution terminates\n"
        "   when the fitness does not change by more than 1%.\n\n"

        "--print-gen <number_of_generations>\n"
        "   Sets the number of generations after which the print condition\n"
        "   will be checked. This option is related to the \"print-change\"\n"
        "   option.\n"
        "   The default value is 10 generations.\n\n"

        "--print-change <value>\n"
        "   The status of the evolution may be printed when the fitness of\n"
        "   the best chromosome in the population decreases by a certain\n"
        "   percentage after a certain number of generations. The percentage\n"
        "   is set by this option. The value is in the interval ]0,1].\n"
        "   The default value is 0.9 meaning that the status will be printed\n"
        "   every time the fitness decreases by 10%.\n\n"

        "--ES (<'plus'> | <'comma'>)\n"
        "   Specifies the selection scheme of evolution strategies.\n"
        "   The default value is 'plus'.\n\n"

        "--max-res <maximum_resistance>\n"
        "   Sets the maximum resistance of the circuit's resistors in ohms.\n"
        "   The default value is 200000R.\n\n"

        "--max-cap <maximum_capacitance>\n"
        "   Sets the maximum capacitance of the circuit's capacitors in nF.\n"
        "   The default value is 500000nF.\n\n"

        "--sigma-init <initial_value>\n"
        "   Sets the initial value of the mutation step.\n"
        "   The default value is 100.\n\n"

        "--fitness (<'bestMatch'> | <'idealSine'> | <'maxAmp'>)\n"
        "   Specifies the evaluation method of chromosomes.\n"
        "   The default value is 'bestMatch'.\n\n"

        "--amplitude <voltage>\n"
        "   Sets the amplitude of the amplifier's output waveform in volts.\n"
        "   This option only applies if the 'idealSine' evaluation method is\n"
        "   used.\n"
        "   The default value is 1V.\n\n"

        "--Rload <resistance>\n"
        "   Sets the resistance of the load resistor for the amplifier in\n"
        "   ohms.\n"
        "   The default value is 22000R.\n\n"

        "--max-diff <difference>\n"
        "   Sets the maximal percentage difference by which the trough and\n"
        "   peak of the amplifier's output waveform may differ. The\n"
        "   difference is in the interval ]0,100].\n"
        "   The default value is 100%.\n\n"

        "--two-stage-amp\n"
        "   Instead of the single stage amplifier, the subject of the\n"
        "   optimization will be the two stage amplifier.\n";

#endif //BT_MAIN_H
