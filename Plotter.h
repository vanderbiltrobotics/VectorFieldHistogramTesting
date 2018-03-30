//
// Created by Swapnil on 2/22/2018.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include "GNUPlot.h"
#include <string>

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
        // TODO: parameterize xrange, yrange according
        int xMax = 60;
        int yMax = 60;
        int pointSize = 7;

        std::vector<std::string> script;
        f.open(fPath);
        f << header << std::endl;
        f.close();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        script.push_back("set xrange [0:" + std::to_string(xMax) + "];");
        script.push_back("set yrange [0:" + std::to_string(yMax) + "];");
        script.push_back("plot \"" + fPath +  "\"" + "with points pointsize " + std::to_string(pointSize) + "");

        plotter.open();
        plotter.execute(script);

    }

    ~Plotter()
    {
        plotter.flush();
        plotter.close();
    }

    void plot(std::vector<discretePoint>& dataIn, int robotX, int robotY, double angle, int timestep, int numTimesteps)
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
        script.push_back("set label 11 center at graph 0.2,char 1 \"t = " + std::to_string(timestep) + " / " + std::to_string(numTimesteps) + " position = (" + std::to_string(robotX) + ", " + std::to_string(robotY) + "); angle = " + std::to_string(angle) + "\" font \",14\"; set bmargin 4;");
        plotter.execute(script);
    }
};
#endif //VECTORFIELDHISTOGRAMTESTING_PLOTTER_H
