# cmake_minimum_required(VERSION <specify CMake version here>)
project(VectorFieldHistogramTesting)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall")

add_executable(VectorFieldHistogramTesting main.cpp HistogramGrid.h Utils.h PolarHistogram.h VFHPather.h)
