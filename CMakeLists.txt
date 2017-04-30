cmake_minimum_required(VERSION 3.6)
project(bt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

set(SOURCE_FILES src/main.cpp src/main.h src/Simulator.cpp src/Simulator.h src/Plotter.cpp src/Plotter.h src/Amplifier.cpp src/Amplifier.h src/Chromosome.cpp src/Chromosome.h src/Population.cpp src/Population.h src/ArgParser.cpp src/ArgParser.h)

include_directories(
        /usr/local/include/ngspice/
        /usr/lib64/R/include
        /usr/lib64/R/library/RInside/include
        /usr/lib64/R/library/Rcpp/include)

link_directories(
        /usr/lib64/R/library/RInside/lib
        /usr/lib64/R/library/Rcpp/libs
        /usr/lib64/R/lib)

add_executable(bt ${SOURCE_FILES})

target_link_libraries( bt
        ngspice
        RInside
        R
        Rcpp)

