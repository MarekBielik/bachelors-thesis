#include "main.h"

int main()
{
    /*pass the callback functions*/
    ngSpice_Init(ng_getchar, ng_getstat, ng_exit, ng_data, ng_initData,
                 NULL, NULL);

    /*load the file with the schematic and simulation commands*/
    ngSpice_Command("source ~/projects/bt/NPN-CEamp/amp.net");

    /*run the simulation*/
    ngSpice_Command("run");

    /*get the current plot*/
    /*(we have only one plot in our schematic)*/
    char * plot =  ngSpice_CurPlot();

    /*get all the vectors in the plot*/
    char ** vectors = ngSpice_AllVecs(plot);

    vector_info voltageVector;
    vector_info timeVector;

    /*run through the vectors*/
    for (size_t i = 0; vectors[i] != 0; i++) {
        pvector_info vector = ngGet_Vec_Info(vectors[i]);

        /*pick the ones named 'out' (according to the schematic) and 'time'*/
        std::string name = vector->v_name;
        if (name == "out")
            voltageVector = *vector;
        else if (name == "time")
            timeVector = *vector;
    }

    /*run through the voltage vector and print out the values*/
    for (size_t i = 0; i < voltageVector.v_length; i++) {
        std::cout << timeVector.v_realdata[i] << " - ";
        std::cout << voltageVector.v_realdata[i] << std::endl;
    }

    ngSpice_Command("quit");

    return 0;
}

/* Callback function called from ngspice to transfer
   any string created by printf or puts. Output to stdout in ngspice is
   preceded by token stdout, same with stderr.*/
int
ng_getchar(char* outputreturn, int ident, void* userdata)
{
    printf("%s\n", outputreturn);
    return 0;
}

/* Callback function called from ngspice to transfer
   simulation status (type and progress in percent. */
int
ng_getstat(char* outputreturn, int ident, void* userdata)
{
    printf("%s\n", outputreturn);
    return 0;
}

/* Callback function called from ngspice if fcn controlled_exit()
   is hit. Do not exit, but unload ngspice. */
int
ng_exit(int exitstatus, bool immediate, bool quitexit, int ident,
        void* userdata)
{
    if(quitexit) {
        printf("DNote: Returned from quit with exit status %d\n", exitstatus);
    }

    if(immediate) {
        printf("DNote: Unload ngspice\n");
        ngSpice_Command("quit");
    }

    return exitstatus;

}

/* Callback function called from ngspice just before the initialization
 * of the simulation vectors*/
int ng_initData(pvecinfoall initData, int id, void * userdata)
{
    return 0;
}

/* Callback function called from ngspice once upon intialization
   of the simulation vectors)*/
int ng_data(pvecvaluesall data, int nOfStructs, int id, void* userdata)
{
    pvecvalues * values = data->vecsa;
    /* this would print the simulation results during runtime */
    /*std::cout << (*values)->name << ": " << (*values)->creal << "  "
                << (*values)->cimag << std::endl;
    */
    return 0;
}

