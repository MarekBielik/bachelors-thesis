//
// Created by marek on 1/24/17.
//

#include "Amplifier.h"

std::vector<std::string> Amplifier::netlistVector;
unsigned Amplifier::netlistSize;
bool Amplifier::twoStageAmp;

void Amplifier::init(Params params) {
    twoStageAmp = params.two_stage_amp;

    if (twoStageAmp)
        netlistSize = TWO_STAGE_NETLIST_SIZE;
    else
        netlistSize = SINGLE_STAGE_NETLIST_SIZE;
}

Amplifier& Amplifier::getInstance() {
    static Amplifier amplifier;
    return amplifier;
}

Amplifier::Amplifier() {
    netlistVector.resize(netlistSize);

    if (!twoStageAmp) {
        netlistVector[0] = "amplifier";
        netlistVector[1] = ".model bc547c NPN (BF=730 NE=1.4 ISE=29.5F IKF=80M "
                "IS=60F VAF=25 ikr=12m BR=10 NC=2 VAR=10 RB=280 RE=1 RC=40 "
                "VJE=.48 tr=.3u tf=.5n cje=12p vje=.48 mje=.5 cjc=6p vjc=.7 "
                "mjc=.33 isc=47.6p kf=2f)";
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
        netlistVector[14] = ".TRAN 20u 1.22m";
        netlistVector[15] = ".end";
    } else {
        netlistVector[0] = "amplifier";
        netlistVector[1] = ".model bc547c NPN (BF=730 NE=1.4 ISE=29.5F IKF=80M "
                "IS=60F VAF=25 ikr=12m BR=10 NC=2 VAR=10 RB=280 RE=1 RC=40 "
                "VJE=.48 tr=.3u tf=.5n cje=12p vje=.48 mje=.5 cjc=6p vjc=.7 "
                "mjc=.33 isc=47.6p kf=2f)";
        netlistVector[2] = "Cm 1 6 1u";
        netlistVector[3] = "Ce 2 0 20u";
        netlistVector[4] = "Ce2 7 8 20u";
        netlistVector[5] = "Q2 5 6 7 bc547c";
        netlistVector[6] = "Rgb 0 8 50";
        netlistVector[7] = "Reb 0 7 1000";
        netlistVector[8] = "Rcb 5 4 4700";
        netlistVector[9] = "R2b 0 6 1000";
        netlistVector[10] = "R1b 6 4 4700";
        netlistVector[11] = "Vin in 0 sin(0 .1 2k)";
        netlistVector[12] = "V1 4 0 12";
        netlistVector[13] = "Cout 5 out 10u";
        netlistVector[14] = "Cin in 3 1u";
        netlistVector[15] = "Rc 1 4 4700";
        netlistVector[16] = "Rload 0 out 22k";
        netlistVector[17] = "Re 0 2 1k";
        netlistVector[18] = "R2 0 3 1k";
        netlistVector[19] = "R1 3 4 4700";
        netlistVector[20] = "Q1 1 3 2 bd547c";
        netlistVector[21] = ".TRAN 20u 1.22m";
        netlistVector[22] = ".end";
    }
}

void Amplifier::setComponentValue(const Component component) {
    if (!twoStageAmp) {
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
                netlistVector[6] = "Cin in 3 " +
                        std::to_string(component.value) + "n";
                break;
            case Ce:
                netlistVector[4] = "Ce 0 5 " + std::to_string(component.value) +
                                   "n";
                break;
            case Cout:
                netlistVector[5] = "Cout 1 out " +
                        std::to_string(component.value) + "n";
                break;
        }
    } else {
        switch (component.name) {
            case Cm:
                netlistVector[2] = "Cm 1 6 " + std::to_string(component.value) +
                                               "n";
                break;
            case Ce:
                netlistVector[3] = "Ce 2 0 " + std::to_string(component.value) +
                                               "n";
                break;
            case Ce2:
                netlistVector[4] = "Ce2 7 8 " +
                                   std::to_string(component.value) + "n";
                break;
            case Rgb:
                netlistVector[6] = "Rbg 0 8 " + std::to_string(component.value);
                break;
            case Reb:
                netlistVector[7] = "Reb 0 7 " + std::to_string(component.value);
                break;
            case Rcb:
                netlistVector[8] = "Rcb 5 4 " + std::to_string(component.value);
                break;
            case R2b:
                netlistVector[9] = "R2b 0 6 " + std::to_string(component.value);
                break;
            case R1b:
                netlistVector[10] = "R1b 6 4 " +
                                    std::to_string(component.value);
                break;
            case Cout:
                netlistVector[13] = "Cout 5 out " +
                                    std::to_string(component.value) + "n";
                break;
            case Cin:
                netlistVector[14] = "Cin in 3 " +
                                    std::to_string(component.value) + "n";
                break;
            case Rc:
                netlistVector[15] = "Rc 1 4 " + std::to_string(component.value);
                break;
            case Re:
                netlistVector[17] = "Re 0 2 " + std::to_string(component.value);
                break;
            case R2:
                netlistVector[18] = "R2 0 3 " + std::to_string(component.value);
                break;
            case R1:
                netlistVector[19] = "R1 3 4 " + std::to_string(component.value);
                break;
        }
    }
}

void Amplifier::setRload(const unsigned value) {
    netlistVector[8] = "Rload 0 out " + std::to_string(value);
}

char ** Amplifier::getNetlist() {
    char **netlistArray;

    if (twoStageAmp)
        netlistArray = singleStageNetlistArray;
    else
        netlistArray = twoStageNetlistArray;

    for (int i = 0; i < netlistSize; i++)
        netlistArray[i] = strdup(netlistVector[i].c_str());

    netlistArray[netlistSize + 1] = NULL;
    return netlistArray;
}

void Amplifier::freeNetlist() {
    char **netlistArray;

    if (twoStageAmp)
        netlistArray = singleStageNetlistArray;
    else
        netlistArray = twoStageNetlistArray;

    for(int i = 0; i < netlistSize; i++) {
        free(netlistArray[i]);
    }
}