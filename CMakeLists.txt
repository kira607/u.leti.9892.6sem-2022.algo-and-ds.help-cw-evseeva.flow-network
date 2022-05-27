cmake_minimum_required(VERSION 3.22)
project(cw_evs)

set(CMAKE_CXX_STANDARD 14)

add_executable(cw_evs
        main.cpp
        graph.cpp log.h)
