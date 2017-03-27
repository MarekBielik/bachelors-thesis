//
// Created by marek on 1/24/17.
//

#ifndef BT_PLOTTER_H
#define BT_PLOTTER_H

#include <vector>
#include <RInside.h>

#define SCREEN_PLOT_MAX 62

class Plotter {
    static std::string path;

    RInside R;
    std::string cmd;
    unsigned plotCount;

public:
    static void init(std::string paramPath = "");

    Plotter();
    //void plot(std::vector<double> time, std::vector<double> voltage);
    void plot(std::vector<double> time, std::vector<double> refInVoltage,
              std::vector<double> voltage);
    void plot(std::vector<double> time, std::vector<double> refVoltage,
              std::vector<double> inVoltage, std::vector<double> outVoltage);
};

#endif //BT_PLOTTER_H
