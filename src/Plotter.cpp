//
// Created by marek on 1/24/17.
//

#include "Plotter.h"

std::string Plotter::path = "";

Plotter::Plotter() {
    plotCount = 0;
}

void Plotter::init(std::string paramPath /*= ""*/) {
    path = paramPath;
}

void Plotter::plot(std::vector<double> time, std::vector<double> voltage) {
    R["time"] = time;
    R["voltageOut"] = voltage;
    plotCount++;

    if (path.length()) {
        cmd = "pdf('" + path + "/" + std::to_string(plotCount) + ".pdf'); "
                "plot(time, voltageOut, type='l', col='green',"
                "     ylab='Voltage [V]', xlab='Time [s]', lwd=2);"
                "grid();"
                "legend('topleft', inset=c(0.25,-0.155), xpd=TRUE,"
                "       legend=c('Evolved solution output'),"
                "       lty=c(1), lwd=c(2), col=c('green'));"
                "dev.off();";
    }
    else {
        if (plotCount > SCREEN_PLOT_MAX)
            return;

        /*to the screen*/
        cmd = "x11();"
                "plot(time, voltageOut, type='l', col='green',"
                "     ylab='Voltage [V]', xlab='Time [s]', lwd=2);"
                "grid();"
                "legend('topleft', inset=c(0.25,-0.155), xpd=TRUE,"
                "       legend=c('Evolved solution output'),"
                "       lty=c(1), lwd=c(2), col=c('green'));";
    }

    R.parseEvalQ(cmd);
}

void Plotter::plot(std::vector<double> time, std::vector<double> refInVoltage,
                   std::vector<double> voltage) {
    R["time"] = time;
    R["refVoltageIn"] = refInVoltage;
    R["voltageOut"] = voltage;
    plotCount++;

    if (path.length()) {
        cmd = "pdf('" + path + "/" + std::to_string(plotCount) + ".pdf'); "
                "plot(time, voltageOut, type='l', col='green',"
                "     ylab='Voltage [V]', xlab='Time [s]', lwd=2);"
                "lines(time, refVoltageIn, col='blue', lwd=2);"
                "grid();"
                "legend('topleft', inset=c(0.25,-0.155), xpd=TRUE,"
                "       legend=c('Evolved solution output', 'input'),"
                "       lty=c(1,1), lwd=c(2,2), col=c('green', 'blue'));"
                "dev.off();";
    }
    else {
        if (plotCount > SCREEN_PLOT_MAX)
            return;

        /*to the screen*/
        cmd = "x11();"
                "plot(time, voltageOut, type='l', col='green',"
                "     ylab='Voltage [V]', xlab='Time [s]', lwd=2);"
                "lines(time, refVoltageIn, col='blue', lwd=2);"
                "grid();"
                "legend('topleft', inset=c(0.25,-0.155), xpd=TRUE,"
                "       legend=c('Evolved solution output', 'input'),"
                "       lty=c(1,1), lwd=c(2,2), col=c('green', 'blue'));";
    }

    R.parseEvalQ(cmd);
}

void Plotter::plot(std::vector<double> time, std::vector<double> refVoltage,
                   std::vector<double> inVoltage,
                   std::vector<double> outVoltage) {

    /*plot a graph of the simulation*/
    R["voltageOut"] = outVoltage;
    R["voltageIn"] = inVoltage;
    R["refVoltageOut"] = refVoltage;
    R["time"] = time;
    plotCount++;

    /*get the boundaries of the plot*/
    int plotSupremum;
    int plotInfimum;

    int tmpOut = ceil(*std::max_element(outVoltage.begin(), outVoltage.end()));
    int tmpRef = ceil(*std::max_element(refVoltage.begin(), refVoltage.end()));

    plotSupremum = tmpOut > tmpRef ? tmpOut : tmpRef;

    tmpOut = floor(*std::min_element(outVoltage.begin(), outVoltage.end()));
    tmpRef = floor(*std::min_element(refVoltage.begin(), refVoltage.end()));

    plotInfimum = tmpOut < tmpRef ? tmpOut : tmpRef;

    if (path.length()) {
        cmd = "pdf('" + path + "/" + std::to_string(plotCount) + ".pdf'); "
                "plot(time, refVoltageOut, type='l', col='red',"
                "     ylab='Voltage [V]', xlab='Time [s]', lwd=2,"
                "     ylim=c(" + std::to_string(plotInfimum) + ", " +
                              std::to_string(plotSupremum) + "));"
                "lines(time, voltageOut, col='green', lwd=2);"
                "lines(time, voltageIn, col='blue', lwd=2);"
                "grid();"
                "legend('topleft', inset=c(0.25,-0.155), xpd=TRUE,"
                "       legend=c('Analytical solution output',"
                "                'Evolved solution output',"
                "                'input'),"
                "lty=c(1,1,1), lwd=c(2,2,2), col=c('red', 'green', 'blue'));"
                "dev.off();";
        /*if we wanted to remove the created file, the command has to
         * have the file name at the end*/
        //"'" + std::to_string(plotCount) + ".png'";

        /*unlink() might be used if we wanted to remove the created file*/
        /*std::string tmpfile = R.parseEval(cmd);*/
        /*unlink(tmpfile.c_str())*/;
    }
    else {
        if (plotCount > SCREEN_PLOT_MAX)
            return;

        /*to the screen*/
        cmd = "x11();"
              "plot(time, refVoltageOut, type='l', col='red',"
              "     ylab='Voltage [V]', xlab='Time [s]', lwd=2,"
              "     ylim=c(" + std::to_string(plotInfimum) + ", " +
                            std::to_string(plotSupremum) + "));"
              "lines(time, voltageOut, col='green', lwd=2);"
              "lines(time, voltageIn, col='blue', lwd=2);"
              "grid();"
              "legend('topleft', inset=c(0.25,-0.155), xpd=TRUE,"
              "       legend=c('Analytical solution output',"
              "                'Evolved solution output',"
              "                'input'),"
              "lty=c(1,1,1), lwd=c(2,2,2), col=c('red', 'green', 'blue'));";
    }

    R.parseEvalQ(cmd);
}