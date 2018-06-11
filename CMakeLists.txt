cmake_minimum_required(VERSION 3.9.1)
# cmake_minimum_required(VERSION <specify CMake version here>)
project(VectorFieldHistogramTesting)

set(CMAKE_CXX_STANDARD 11)


add_executable(VectorFieldHistogramTesting main.cpp HistogramGrid.h Utils.h PolarHistogram.h VFHPather.h RobotTest.h
        GNUPlot.h Plotter.h GNUPlot.cpp)
