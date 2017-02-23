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
    netlistVector[3] = "V1 4 0 9";
    netlistVector[4] = "Ce 0 5 5u";
    netlistVector[5] = "Cout 1 out 220n";
    netlistVector[6] = "Cin in 3 220n";
    netlistVector[7] = "Rc 1 4 1000";
    netlistVector[8] = "Rload 0 out 22000";
    netlistVector[9] = "Re 0 5 1000";
    netlistVector[10] = "Rg 5 2 40";
    netlistVector[11] = "R3 0 3 33000";
    netlistVector[12] = "R1 3 4 47000";
    netlistVector[13] = "Q1 1 3 2 bc547c";
    netlistVector[14] = ".TRAN 5u 1.23m";
    netlistVector[15] = ".end";
}

void Amplifier::setR1(const uint32_t resistance) {
    netlistVector[12] = "R1 3 4 " + std::to_string(resistance);
}

char ** Amplifier::getNetlist() {
   /* netlist[0] = strdup(description.c_str());
    netlist[1] = strdup(bc547c.c_str());
    netlist[2] = strdup(Vin.c_str());
    netlist[3] = strdup(V1.c_str());
    netlist[4] = strdup(Ce.c_str());
    netlist[5] = strdup(Cout.c_str());
    netlist[6] = strdup(Cin.c_str());
    netlist[7] = strdup(Rc.c_str());
    netlist[8] = strdup(Rload.c_str());
    netlist[9] = strdup(Re.c_str());
    netlist[10] = strdup(Rg.c_str());
    netlist[11] = strdup(R2.c_str());
    netlist[12] = strdup(R1.c_str());
    netlist[13] = strdup(Q1.c_str());
    netlist[14] = strdup(tran.c_str());
    netlist[15] = strdup(end.c_str());
    netlist[16] = NULL;
    */

    for (int i = 0; i < NETLIST_SIZE; i++)
        netlistArray[i] = strdup(netlistVector[i].c_str());

    netlistArray[NETLIST_SIZE + 1] = NULL;
    return netlistArray;
}

/*todo: this function probably won't be needed*/
void Amplifier::freeNetlist() {
    for(int i = 0; i < NETLIST_SIZE; i++) {
        free(netlistArray[i]);
    }
}