#!/bin/bash

test=0;
rm -rf experiments/two_stage_amp/

for mu in 1 5 10 15; do
    for selectivePressure in 1 5 10 15; do
        lambda=$((mu * selectivePressure));

        #pick the right number of generations for the stop condition
        if [ "$lambda" -lt "10" ]; then
            stop_gen=3000;
        elif [ "$lambda" -lt "50" ]; then
            stop_gen=500;
        elif [ "$lambda" -lt "100" ]; then
            stop_gen=400;
        elif [ "$lambda" -lt "200" ]; then
            stop_gen=300;
        else
            stop_gen=200;
        fi

        for ES in comma plus; do
            #skip certain experiments
            if [ "$ES" == "comma" ] && [ "$lambda" -eq "1" ]; then
                continue
            fi

            for fitness in idealSine maxAmp; do
                avg=0;
                ((test++));

                for i in {1..10}; do
                    path="experiments/two_stage_amp/mu$mu/lambda$lambda/$ES/$fitness/$i";
                    mkdir -p ${path};
                    result=`./build/bt -o ${path} --mu=${mu} --lambda=${lambda} --max_gen=20000 --stop_gen=${stop_gen} --print_gen=1 --print_change=0.95 --ES=${ES} --max_res=200000 --fitness=${fitness} --amplitude=3.5 --max_diff=25 --two_stage_amp`;
                    echo ${result} | tee -a experiments/two_stage_amp/results.txt;
                    avg=$(bc -l <<< "$avg + $result");
                done

                avg=$(bc -l <<< "$avg / 10");
                echo "experiment $test: $path" | tee -a experiments/two_stage_amp/results.txt;
                echo "average: $avg" | tee -a experiments/two_stage_amp/results.txt;
            done
        done
    done
done

echo "Experiments done.";h