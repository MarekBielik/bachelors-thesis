//
// Created by marek on 1/24/17.
//

#ifndef BT_SIMULATOR_H
#define BT_SIMULATOR_H

#include <sharedspice.h>
#include <iostream>
#include <vector>
#include <ctime>

#define COPY_LENGTH 69

class Simulator {
    /*vectors returned by ngSPICE*/
    char **vectors;

    void run(char *circuit[]);
    void unload();
    pvector_info getVector(std::string name);

public:
    Simulator();
    void simulate(char *circuit[], std::vector<double> &voltageOut,
                  std::vector<double> &time, std::vector<double> &voltageIn);
    void simulate(char *circuit[], std::vector<double> &voltageOut,
                  std::vector<double> &time, bool clean = true);
    void simulate(char *circuit[], std::vector<double> &voltageOut,
                  bool clean = true);
};

/*ngSPICE helper function*/
ControlledExit ng_exit;

#endif //BT_SIMULATOR_H
