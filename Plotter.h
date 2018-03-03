//
// Created by Swapnil on 2/22/2018.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include "GNUPlot.h"


#ifndef VECTORFIELDHISTOGRAMTESTING_PLOTTER_H
#define VECTORFIELDHISTOGRAMTESTING_PLOTTER_H

class Plotter {
private:
    std::ofstream f;
    std::string fPath = "tempdata.dat";
    std::string header = "# x   y";
    GNUPlot plotter;

public:
    Plotter()
    {
        std::vector<std::string> script;
        f.open(fPath);
        f << header << std::endl;
        f.close();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));


        script.push_back("set xrange [0:60];");
        script.push_back("set yrange [0:60];");
        script.push_back("plot \"" + fPath +  "\"" + "with points pointsize 7");

        plotter.open();
        plotter.execute(script);

    }

    ~Plotter()
    {
        plotter.flush();
        plotter.close();
    }

    void plot(std::vector<discretePoint>& dataIn)
    {
        std::vector<std::string> script;
        script.push_back("replot");

        f.open(fPath);
        f << header << std::endl;
        for(int j = 0; j < dataIn.size(); j++)
        {
            f << dataIn[j].x << " " << dataIn[j].y << std::endl;
        }
        f.close();
        plotter.execute(script);
    }
};
#endif //VECTORFIELDHISTOGRAMTESTING_PLOTTER_H
