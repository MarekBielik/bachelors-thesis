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

#endif //BT_MAIN_H
