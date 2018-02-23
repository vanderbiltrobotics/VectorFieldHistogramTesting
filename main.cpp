#include <iostream>
#include "HistogramGrid.h"
#include "PolarHistogram.h"
#include "Utils.h"
#include "VFHPather.h"
#include "RobotTest.h"

#include <chrono>
#include <thread>
#include "Plotter.h"


int getIndex(int a, int n) {
    return ((a % n) + n) % n;
}

int main()
{
    Plotter plotter;
    std::vector<std::vector<int>> data(2, std::vector<int>(0, 0));
    for(int i = 0; i < 500; i++)
    {
        data[0].push_back(250);
        data[1].push_back(250);

        std::cout << i << std::endl;
        plotter.plot(data);
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

//    int x_init = 25;
//    int y_init = 25;
//    // double angle_init = 0.78; // pi/4
//    double angle_init = 90; // pi/4
//    double speed_init = 0.5;
//
//    RobotTest bot(x_init, y_init, angle_init, speed_init);
//
//    int timestep = 100;
//
//    GNUPlot gnuplot;
//
//    for(int i = 0; i < timestep; ++i)
//    {
//      std::cout<<i<<"th timestep: ";
//      bot.move();
//      // bot.talk();
//      // bot.draw(gnuplot);
//      bot.draw(gnuplot);
//      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//
//    }
//    // VFHPather pather;
//    // discretePoint robotLoc;
//    // robotLoc.x = 25;
//    // robotLoc.y = 25;
//
//    // pather.updateRobotPosition(robotLoc);
//    //pather.generateHistogram();
//    // std:: cout << pather.computeTravelDirection();



}
