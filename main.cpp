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
//    Plotter plotter;
//    std::vector<std::vector<int>> data(2, std::vector<int>(0, 0));
//    data[0].push_back(0);
//    data[1].push_back(0);
//    for(int i = 0; i < 500; i++)
//    {
//        data[0][0] = i;
//        data[1][0] = i;
//
//        std::cout << i << std::endl;
//        plotter.plot(data);
//    }


    discretePoint init;
    init.x = 30;
    init.y = 30;

    // double angle_init = 0.78; // pi/4
    double angle_init = 90; // pi/4
    double speed_init = 0.5;

    RobotTest bot(init, angle_init, speed_init);
    getchar();


    int timestep = 100;


    for(int i = 0; i < timestep; ++i)
    {
      std::cout<<i<<"th timestep: ";
      bot.move();
      bot.talk();
      // bot.draw(gnuplot);
      bot.draw();
      std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::cout << "\n\n\n\n\n\n";
    }
//    // VFHPather pather;
//    // discretePoint robotLoc;
//    // robotLoc.x = 25;
//    // robotLoc.y = 25;
//
//    // pather.updateRobotPosition(robotLoc);
//    //pather.generateHistogram();
//    // std:: cout << pather.computeTravelDirection();

}
