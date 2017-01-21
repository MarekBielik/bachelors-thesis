cmake_minimum_required(VERSION 3.6)
project(bt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories( /usr/share/ngspice/include/ngspice/)
set(SOURCE_FILES main.cpp main.h)
add_executable(bt ${SOURCE_FILES})
target_link_libraries( bt /usr/local/lib/libngspice.so)