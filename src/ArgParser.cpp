//
// Created by marek on 3/18/17.
//

#include "ArgParser.h"

ArgParser::ArgParser() {

}

void ArgParser::parse(int argc, char **argv) {
    double maxDiff = DEFAULT_DIFF;

    params.mu = MU;
    params.lamda = LAMBDA;
    params.max_gen = MAX_GEN;
    params.stop_gen = STOP_GEN;
    params.print_gen = PRINT_GEN;
    params.print_change = PRINT_CHANGE;
    params.stop_change = STOP_CHANGE;
    params.ES_type = "plus";
    params.max_res = MAX_RESISTANCE;
    params.max_cap = MAX_CAPACITY;
    params.sigma_init = SIGMA_INIT;
    params.objFunType = "bestFit";
    params.path = "";
    params.amplitude = DEFAULT_AMPLITUDE;
    params.Rload = R_LOAD;

    int c;
    int option_index = 0;

    while ((c = getopt_long(argc, argv, short_options, long_options,
                       &option_index)) != -1) {
            switch (c) {
                case mu:
                    params.mu = std::stoul(optarg);
                    break;
                case lambda:
                    params.lamda = std::stoul(optarg);
                    break;
                case max_gen:
                    params.max_gen = std::stoul(optarg);
                    break;
                case stop_gen:
                    params.stop_gen = std::stoul(optarg);
                    break;
                case print_gen:
                    params.print_gen = std::stoul(optarg);
                    break;
                case print_change:
                    params.print_change = std::stod(optarg);
                    break;
                case stop_change:
                    params.stop_change = std::stod(optarg);
                    break;
                case ES_type:
                    params.ES_type = std::string(optarg);
                    break;
                case max_res:
                    params.max_res = std::stoul(optarg);
                    break;
                case max_cap:
                    params.max_cap = std::stoul(optarg);
                    break;
                case sigma_init:
                    params.sigma_init = std::stoul(optarg);
                    break;
                case obj_fun_type:
                    params.objFunType = std::string(optarg);
                    break;
                case 'o':
                    params.path = std::string(optarg);
                    break;
                case amp:
                    params.amplitude = std::stod(optarg);
                    break;
                case Rload:
                    params.Rload = std::stoul(optarg);
                    break;
                case max_diff:
                    maxDiff = std::stoi(optarg);
                    break;
                case '?':
                default:
                    throw std::invalid_argument("Invalid arguments syntax");
            }
    }

    /*a simple parameters validity check*/
    if (params.mu > params.lamda ||
            params.print_change > 1.0 ||
            params.stop_change > 1.0 ||
            params.amplitude < 0 ||
            maxDiff < 0 ||
            maxDiff > 100 ||
            (params.ES_type != "plus" && params.ES_type != "comma") ||
            (params.objFunType != "bestFit" &&
                     params.objFunType != "idealSine" &&
                     params.objFunType != "maxAmp") ||
            (params.path.length() && access(params.path.c_str(), W_OK) != 0) ||
            optind != argc){
        throw std::invalid_argument("Invalid arguments.");
    }

    /*transform the difference from percentage to an inverted interval
     * <0, 1>*/
    params.max_diff = 1 - (maxDiff / 100);
}

const Params & ArgParser::getParams() const {
    return params;
}

bool ArgParser::screenOutput() const {
    return params.path.length() == 0;
}