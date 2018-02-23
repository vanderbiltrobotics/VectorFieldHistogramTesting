cmake_minimum_required(VERSION 3.9.6)
# cmake_minimum_required(VERSION <specify CMake version here>)
project(VectorFieldHistogramTesting)

set(CMAKE_CXX_STANDARD 11)

# add_executable(VectorFieldHistogramTesting main.cpp HistogramGrid.h Utils.h PolarHistogram.h VFHPather.h)
add_executable(VectorFieldHistogramTesting main.cpp HistogramGrid.h Utils.h PolarHistogram.h VFHPather.h GNUPlot.h GNUPlot.cpp Plotter.h)

# set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)
# find_package(Boost 1.66.0 COMPONENTS system file_descriptor.hpp stream.hpp)
#
# if(Boost_FOUND)
#     include_directories(${Boost_INCLUDE_DIRS})
#     add_executable(VectorFieldHistogramTesting main.cpp HistogramGrid.h Utils.h PolarHistogram.h VFHPather.h)
#     # add_executable(progname file1.cxx file2.cxx)
#     target_link_libraries(VectorFieldHistogramTesting ${Boost_LIBRARIES})
# endif()
