//
// Created by marek on 1/24/17.
//

#include "Simulator.h"

Simulator::Simulator() {
    /*pass the callback functions*/
    ngSpice_Init(NULL, NULL, ng_exit, NULL, NULL,
                 NULL, NULL);
}

/*some parts are commented out and replaced by static values
 * due to optimizations*/
void Simulator::simulate(char **circuit, std::vector<double> &voltageOut,
                         std::vector<double> &time,
                         std::vector<double> &voltageIn) {
    simulate(circuit, voltageOut, time, false);

    /*vector_info vector = *getVector("in");*/
    vector_info vector = *ngGet_Vec_Info(vectors[8]);

    voltageIn.assign(vector.v_realdata, vector.v_realdata + COPY_LENGTH);

    unload();
}

void Simulator::simulate(char **circuit, std::vector<double> &voltageOut,
                         std::vector<double> &time, bool clean /*= true*/) {

    simulate(circuit, voltageOut, false);

    /*vector_info vector = *getVector("time");*/
    vector_info vector = *ngGet_Vec_Info(vectors[9]);

    time.assign(vector.v_realdata, vector.v_realdata + COPY_LENGTH);

    if (clean) unload();
}

void Simulator::simulate(char **circuit, std::vector<double> &voltageOut,
                         bool clean /*= true*/) {
    run(circuit);

    /*vector_info vector = *getVector("out");*/
    vector_info vector = *ngGet_Vec_Info(vectors[4]);

    voltageOut.assign(vector.v_realdata, vector.v_realdata + COPY_LENGTH);

    if (clean) unload();
}

pvector_info Simulator::getVector(std::string name) {
    /*find the appropriate vector according to it's name*/
    for (size_t i = 0; vectors[i] != 0; i++) {
        pvector_info vector = ngGet_Vec_Info(vectors[i]);
        std::string vectorName = vector->v_name;

        if (vectorName == name)
            return vector;
    }

    return NULL;
}

void Simulator::run(char **circuit) {
    /*load the shematic and run the simulation*/
    ngSpice_Circ(circuit);

    ngSpice_Command((char*) "run");

    /*get the current plot*/
    /*char * plot =  ngSpice_CurPlot();*/

    /*get all the vectors in the plot*/
    vectors = ngSpice_AllVecs(/*plot*/ (char*) "tran1");
}

void Simulator::unload() {
    ngSpice_FreeResources();
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