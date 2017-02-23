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
                         std::vector<double> *voltage,
                         std::vector<double> *time /*= NULL*/) {



    /*load the shematic and run the simulation*/
    ngSpice_Circ(circuit);

    /*todo:test code*/
    static unsigned simulationCount = 0;
    clock_t start;
    if((++simulationCount % 100) == 0) {
        start = clock();
        ngSpice_Command("run");
        std::cout << "Simulation no. " << simulationCount << ", time: "
                  << double(clock() - start) / CLOCKS_PER_SEC << std::endl;
    } else
        ngSpice_Command("run");


    /*get the current plot*/
    char * plot =  ngSpice_CurPlot();

    /*get all the vectors in the plot*/
    char ** vectors = ngSpice_AllVecs(plot);

    vector_info voltageVector;
    vector_info timeVector;

    /*find the time and voltage vectors*/
    for (size_t i = 0; vectors[i] != 0; i++) {
        pvector_info vector = ngGet_Vec_Info(vectors[i]);

        /*pick the ones named 'out' (according to the schematic) and 'time'*/
        std::string name = vector->v_name;
        if (name == "out")
            voltageVector = *vector;
        else if ( time && (name == "time"))
            timeVector = *vector;
    }

    if (time)
        time->assign(timeVector.v_realdata,
                timeVector.v_realdata + timeVector.v_length);

    voltage->assign(voltageVector.v_realdata,
                   voltageVector.v_realdata + voltageVector.v_length);

    //ngSpice_Command("destroy all");
    ngSpice_Command("rehash");
    ngSpice_Command("reset");
    ngSpice_Command("remcirc");

    /*todo: rename the function*/
    myFreeFunc();
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
        ngSpice_Command("quit");
    }

    return exitstatus;
}