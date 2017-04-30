//
// Created by marek on 3/18/17.
//

#ifndef BT_ARGPARSER_H
#define BT_ARGPARSER_H

#include <getopt.h>

#include "Population.h"
#include "Amplifier.h"

#define DEFAULT_DIFF 100;

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
    obj_fun_type,
    amp,
    Rload,
    max_diff,
    two_stage_amp
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
    double amplitude;
    unsigned Rload;
    double max_diff;
    bool two_stage_amp;
};

class ArgParser {
    const struct option long_options[18] = {
            {"help", no_argument, 0, 'h'},
            {"mu", required_argument, 0, mu},
            {"lambda", required_argument, 0, lambda},
            {"max-gen", required_argument, 0, max_gen},
            {"stop-gen", required_argument, 0, stop_gen},
            {"print-gen", required_argument, 0, print_gen},
            {"print-change", required_argument, 0, print_change},
            {"stop-change", required_argument, 0, stop_change},
            {"ES", required_argument, 0, ES_type},
            {"max-res", required_argument, 0, max_res},
            {"max-cap", required_argument, 0, max_cap},
            {"sigma-init", required_argument, 0, sigma_init},
            {"fitness", required_argument, 0, obj_fun_type},
            {"amplitude", required_argument, 0, amp},
            {"Rload", required_argument, 0, Rload},
            {"max-diff", required_argument, 0, max_diff},
            {"two-stage-amp", no_argument, 0, two_stage_amp},
            {              0,           0, 0, 0}
    };

    const char * short_options = "ho:";

    Params params;

public:
    ArgParser();
    bool screenOutput() const;
    void parse(int argc, char ** argv);
    const Params & getParams() const;
};

#endif //BT_ARGPARSER_H
