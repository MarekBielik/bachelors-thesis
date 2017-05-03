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
    /**
     * Names of 'vectors' returned by ngSPICE.
     */
    char **vectors;

    /**
     * Runs the simulator.
     * @param circuit the circuit to simulate
     */
    void run(char *circuit[]);

    /**
     * Unloads the simulator, this method is called between the simulation runs.
     */
    void unload();

    /**
     * Finds the 'ngSPICE' vector according to its name.
     * @param name
     * @return the found vector
     */
    pvector_info getVector(std::string name);

public:
    Simulator();

    /**
     * Simulates the given amplifier.
     * @param circuit the circuit to simulate
     * @param voltageOut the output voltage of the amplifier
     * @param time contains the time of the simulation
     * @param voltageIn the input voltage of the amplifier
     */
    void simulate(char *circuit[], std::vector<double> &voltageOut,
                  std::vector<double> &time, std::vector<double> &voltageIn);

    /**
     * Simulates the given amplifier.
     * @param circuit the circuit to simulate
     * @param voltageOut the output voltage of the amplifier
     * @param time the time of the simulation
     * @param clean if true, the simulator will be unloaded
     */
    void simulate(char *circuit[], std::vector<double> &voltageOut,
                  std::vector<double> &time, bool clean = true);

    /**
     * Simulates the given amplifier.
     * @param circuit the circuit to simulate
     * @param voltageOut the output voltage of the amplifier
     * @param clean if true, the simulator will be unloaded
     */
    void simulate(char *circuit[], std::vector<double> &voltageOut,
                  bool clean = true);
};

/**
 * ngSPICE helper function, it is mandatory to pass a pointer to this function
 * to the simulator.
 */
ControlledExit ng_exit;

#endif //BT_SIMULATOR_H
