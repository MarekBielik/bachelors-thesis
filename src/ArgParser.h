//
// Created by marek on 3/18/17.
//

#ifndef BT_ARGPARSER_H
#define BT_ARGPARSER_H

#include <getopt.h>

#include "Population.h"

enum Options {
    mu,
    lambda,
    max_gen,
    stop_gen,
    print_gen,
    print_change,
    stop_change,
    ES_type,
    max_res,
    max_cap,
    sigma_init,
    obj_fun_type
};

struct Params {
    unsigned mu;
    unsigned lamda;
    unsigned max_gen;
    unsigned stop_gen;
    unsigned print_gen;
    double print_change;
    double stop_change;
    std::string ES_type;
    unsigned max_res;
    unsigned max_cap;
    unsigned sigma_init;
    std::string objFunType;
    std::string path;
};

class ArgParser {
    const struct option long_options[12] = {
        {"mu", required_argument, 0, mu},
        {"lambda", required_argument, 0, lambda},
        {"max_gen", required_argument, 0, max_gen},
        {"stop_gen", required_argument, 0, stop_gen},
        {"print_gen", required_argument, 0, print_gen},
        {"print_change", required_argument, 0, print_change},
        {"stop_change", required_argument, 0, stop_change},
        {"ES_type", required_argument, 0, ES_type},
        {"max_res", required_argument, 0, max_res},
        {"max_cap", required_argument, 0, max_cap},
        {"sigma_init", required_argument, 0, sigma_init},
        {"obj_fun_type", required_argument, 0, obj_fun_type}
};

    const char * short_options = "o:";

    Params params;

public:
    ArgParser();
    bool screenOutput() const;
    void parse(int argc, char ** argv);
    const Params & getParams() const;
};


#endif //BT_ARGPARSER_H
