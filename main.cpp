#include <iostream>
#include "HistogramGrid.h"
#include "PolarHistogram.h"
#include "Utils.h"
#include "VFHPather.h"
#include "RobotTest.h"

int getIndex(int a, int n) {
    return ((a % n) + n) % n;
}

int main()
{
    int x_init = 25;
    int y_init = 25;
    // double angle_init = 0.78; // pi/4
    double angle_init = 90; // pi/4
    double speed_init = 0.5;

    RobotTest bot(x_init, y_init, angle_init, speed_init);

    int timestep = 100;

    for(int i = 0; i < timestep; ++i)
    {
      std::cout<<i<<"th timestep: ";
      bot.move();
      bot.talk();
    }
    // VFHPather pather;
    // discretePoint robotLoc;
    // robotLoc.x = 25;
    // robotLoc.y = 25;

    // pather.updateRobotPosition(robotLoc);
    //pather.generateHistogram();
    // std:: cout << pather.computeTravelDirection();



}
