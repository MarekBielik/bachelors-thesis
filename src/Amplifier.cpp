//
// Created by marek on 1/24/17.
//

#include "Amplifier.h"

std::vector<std::string> Amplifier::netlistVector;

Amplifier::Amplifier() {
    netlistVector.resize(NETLIST_SIZE);

    netlistVector[0] = "amplifier";
    netlistVector[1] = ".model bc547c NPN (BF=730 NE=1.4 ISE=29.5F IKF=80M "
            "IS=60F VAF=25 ikr=12m BR=10 NC=2 VAR=10 RB=280 RE=1 RC=40 VJE=.48 "
            "tr=.3u tf=.5n cje=12p vje=.48 mje=.5 cjc=6p vjc=.7 mjc=.33 "
            "isc=47.6p kf=2f)";
    netlistVector[2] = "Vin in 0 sin(0 .1 2k)";
    netlistVector[3] = "V1 4 0 12";
    netlistVector[4] = "Ce 0 5 5u";
    netlistVector[5] = "Cout 1 out 220n";
    netlistVector[6] = "Cin in 3 220n";
    netlistVector[7] = "Rc 1 4 1000";
    netlistVector[8] = "Rload 0 out 22000";
    netlistVector[9] = "Re 0 5 1000";
    netlistVector[10] = "Rg 5 2 40";
    netlistVector[11] = "R2 0 3 33000";
    netlistVector[12] = "R1 3 4 47000";
    netlistVector[13] = "Q1 1 3 2 bc547c";
    netlistVector[14] = ".TRAN 20u 1.24m";
    netlistVector[15] = ".end";
}

void Amplifier::setComponentValue(const Component component) {
    switch (component.name) {
        case R1:
            netlistVector[12] = "R1 3 4 " + std::to_string(component.value);
            break;
        case R2:
            netlistVector[11] = "R2 0 3 " + std::to_string(component.value);
            break;
        case Re:
            netlistVector[9] = "Re 0 5 " + std::to_string(component.value);
            break;
        case Rg:
            netlistVector[10] = "Rg 5 2 " + std::to_string(component.value);
            break;
        case Rc:
            netlistVector[7] = "Rc 1 4 " + std::to_string(component.value);
            break;
        case Cin:
            netlistVector[6] = "Cin in 3 " + std::to_string(component.value) +
                                             "n";
            break;
        case Ce:
            netlistVector[4] = "Ce 0 5 " + std::to_string(component.value) +
                                           "n";
            break;
    }
}

void Amplifier::setRload(const unsigned value) {
    netlistVector[8] = "Rload 0 out " + std::to_string(value);
}

char ** Amplifier::getNetlist() {

    for (int i = 0; i < NETLIST_SIZE; i++)
        netlistArray[i] = strdup(netlistVector[i].c_str());

    netlistArray[NETLIST_SIZE + 1] = NULL;
    return netlistArray;
}

void Amplifier::freeNetlist() {
    for(int i = 0; i < NETLIST_SIZE; i++) {
        free(netlistArray[i]);
    }
}