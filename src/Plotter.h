//
// Created by marek on 1/24/17.
//

#ifndef BT_PLOTTER_H
#define BT_PLOTTER_H

#include <vector>
#include <RInside.h>

#define SCREEN_PLOT_MAX 61

class Plotter {
    static std::string path;

    RInside R;
    std::string cmd;
    unsigned plotCount;

    Plotter();
    Plotter(const Plotter&);
    Plotter& operator=(const Plotter&);

public:
    static void init(std::string paramPath = "");
    static Plotter& getInstance();

    void plot(std::vector<double> &time, std::vector<double> &voltage);
    void plot(std::vector<double> &time, std::vector<double> &refInVoltage,
              std::vector<double> &voltage);
    void plot(std::vector<double> &time, std::vector<double> &refVoltage,
              std::vector<double> &inVoltage, std::vector<double> &outVoltage);
    void plotEvolutionCourse(std::vector<double> & bestChromosomes,
              std::vector<double> & averageChromosomes,
              std::vector<double> & worstChromosomes,
              std::vector<unsigned> & generations);
};

#endif //BT_PLOTTER_H
