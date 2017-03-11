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

class Amplifier {
    char * netlistArray[NETLIST_SIZE + 1];
    static std::vector<std::string> netlistVector;

public:
    Amplifier();
    char ** getNetlist();
    void freeNetlist();
    /*todo: write a generic method*/
    void setR1(const uint32_t resistance);
    void setR2(const uint32_t resistance);
    void setRe(const uint32_t resistance);
    void setRg(const uint32_t resistance);
};


#endif //BT_CIRCUIT_H
