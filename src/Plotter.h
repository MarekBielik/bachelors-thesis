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
    /**
     * This method initializes the class, it has to be called before its use.
     * @param paramPath contains the path to the directory where the graphs will
     * be stored. If empty, the graphs will be displayed on the screen.
     */
    static void init(std::string paramPath = "");

    /**
     * Returns an instance of Plotter.
     * @return instance of Plotter
     */
    static Plotter& getInstance();

    /**
     * Plots the amplifier's waveform.
     * @param time data for the x coordinate
     * @param voltage data for the y coordinate
     */
    void plot(std::vector<double> &time, std::vector<double> &voltage);

    /**
     * Plots the amplifier's waveform.
     * @param time data for the x coordinate
     * @param refInVoltage data for the y coordinate
     * @param voltage data for the y coordinate
     */
    void plot(std::vector<double> &time, std::vector<double> &refInVoltage,
              std::vector<double> &voltage);

    /**
     * Plots the amplifier's waveform.
     * @param time data for the x coordinate
     * @param refVoltage data for the y coordinate
     * @param inVoltage data for the y coordinate
     * @param outVoltage data for the y coordinate
     */
    void plot(std::vector<double> &time, std::vector<double> &refVoltage,
              std::vector<double> &inVoltage, std::vector<double> &outVoltage);

    /**
     * Plots the course of the evolution.
     * @param bestChromosomes data for the y coordinate
     * @param averageChromosomes data for the y coordinate
     * @param worstChromosomes data for the y coordinate
     * @param generations data for the x coordinate
     */
    void plotEvolutionCourse(std::vector<double> & bestChromosomes,
              std::vector<double> & averageChromosomes,
              std::vector<double> & worstChromosomes,
              std::vector<unsigned> & generations);
};

#endif //BT_PLOTTER_H
