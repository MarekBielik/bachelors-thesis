//
// Created by marek on 1/24/17.
//

#ifndef BT_CIRCUIT_H
#define BT_CIRCUIT_H

#include <string>
#include <string.h>
#include <cstdint>
#include <vector>
#include "ArgParser.h"

/*forward declarations*/
struct Params;

#define SINGLE_STAGE_NETLIST_SIZE 16
#define TWO_STAGE_NETLIST_SIZE 23
#define R_LOAD 22000

enum ComponentName {
    R1,
    R2,
    Re,
    Rg,
    Rc,
    Rgb,
    Reb,
    Rcb,
    R2b,
    R1b,
    Cin,
    Cout,
    Ce,
    Cm,
    Ce2
};

enum ComponentType {
    resistor,
    capacitor
};

struct Component {
    ComponentName name;
    ComponentType type;
    int32_t value;
};

class Amplifier {
    static unsigned netlistSize;
    static std::vector<std::string> netlistVector;
    static bool twoStageAmp;

    char * singleStageNetlistArray[SINGLE_STAGE_NETLIST_SIZE + 1];
    char * twoStageNetlistArray[TWO_STAGE_NETLIST_SIZE + 1];

public:
    static void init(Params params);
    static Amplifier& getInstance();

    Amplifier();
    char ** getNetlist();
    void freeNetlist();
    void setComponentValue(const Component component);
    void setRload(const unsigned value);
};


#endif //BT_CIRCUIT_H
