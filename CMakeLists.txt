cmake_minimum_required(VERSION 3.6)
project(bt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(
        /usr/share/ngspice/include/ngspice/
        /usr/lib64/R/include
        /usr/lib64/R/library/RInside/include
        /usr/lib64/R/library/Rcpp/include)
link_directories(
        /usr/lib64/R/library/RInside/lib
        /usr/lib64/R/library/Rcpp/libs
        /usr/lib64/R/lib
        /usr/local/lib
)
set(SOURCE_FILES main.cpp main.h)
add_executable(bt ${SOURCE_FILES})
target_link_libraries( bt
        ngspice
        RInside
        R
        Rcpp)