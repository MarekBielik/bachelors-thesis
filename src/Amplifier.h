//
// Created by marek on 1/24/17.
//

#ifndef BT_CIRCUIT_H
#define BT_CIRCUIT_H

#include <string>
#include <string.h>
#include <cstdint>
#include <vector>

#define NETLIST_SIZE 16

enum ComponentName {
    R1,
    R2,
    Re,
    Rg
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
    char * netlistArray[NETLIST_SIZE + 1];
    static std::vector<std::string> netlistVector;

public:
    Amplifier();
    char ** getNetlist();
    void freeNetlist();
    void setComponentValue(const Component component);
};


#endif //BT_CIRCUIT_H
