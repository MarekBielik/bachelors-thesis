//
// Created by marek on 1/24/17.
//

#ifndef BT_SIMULATOR_H
#define BT_SIMULATOR_H

#include <sharedspice.h>
#include <iostream>
#include <vector>
#include <ctime>

class Simulator {

public:
    Simulator();
    void simulate(char *circuit[], std::vector<double> *voltage,
                  std::vector<double> *time = NULL);
};

/*ngSPICE helper function*/
ControlledExit ng_exit;

#endif //BT_SIMULATOR_H
