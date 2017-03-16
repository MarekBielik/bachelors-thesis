//
// Created by marek on 1/24/17.
//

#include "Simulator.h"

Simulator::Simulator() {
    /*pass the callback functions*/
    ngSpice_Init(NULL, NULL, ng_exit, NULL, NULL,
                 NULL, NULL);
}

void Simulator::simulate(char **circuit,
                         std::vector<double> *voltageOut,
                         std::vector<double> *voltageIn,
                         std::vector<double> *time /*= NULL*/) {

    /*load the shematic and run the simulation*/
    ngSpice_Circ(circuit);

    ngSpice_Command((char*) "run");

    /*get the current plot*/
    /*char * plot =  ngSpice_CurPlot();*/

    /*get all the vectors in the plot*/
    char ** vectors = ngSpice_AllVecs(/*plot*/ (char*) "tran1");

    vector_info voltageOutVector;
    vector_info voltageInVector;
    vector_info timeVector;

    /*find the time and voltage vectors*/
//    for (size_t i = 0; vectors[i] != 0; i++) {
//        pvector_info vector = ngGet_Vec_Info(vectors[i]);
//
//        /*pick the ones named 'out' (according to the schematic) and 'time'*/
//        std::string name = vector->v_name;
//        if (name == "out")
//            voltageOutVector = *vector;
//        else if (time && (name == "time"))
//            timeVector = *vector;
//        else if (voltageIn && (name == "in"))
//            voltageInVector = *vector;
//    }

    /*todo: possible optimization - set the right length to copy*/
    if (time) {
        timeVector = *ngGet_Vec_Info(vectors[9]);
        time->assign(timeVector.v_realdata,
                     timeVector.v_realdata + 70);
    }

    if (voltageIn) {
        voltageInVector = *ngGet_Vec_Info(vectors[8]);
        voltageIn->assign(voltageInVector.v_realdata,
                          voltageInVector.v_realdata + 70);
    }

    voltageOutVector = *ngGet_Vec_Info(vectors[4]);
    voltageOut->assign(voltageOutVector.v_realdata,
                   voltageOutVector.v_realdata + 70);

    ngSpice_free_resources();
    ngSpice_Command((char*) "destroy tran1");
    ngSpice_Command((char*) "remcirc");
}

/* Callback function called from ngspice if fcn controlled_exit()
   is hit. Do not exit, but unload ngspice. */
int
ng_exit(int exitstatus, bool immediate, bool quitexit, int ident,
        void* userdata)
{
    std::cerr << "NgSPICE has finished, exit status: " << exitstatus;

    if(quitexit) {
        std::cerr << ", no runtime error";
    }

    std::cerr << std::endl;

    if(immediate) {
        printf("DNote: Unload ngspice\n");
        ngSpice_Command((char*) "quit");
    }

    return exitstatus;
}