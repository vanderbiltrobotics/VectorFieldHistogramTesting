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
    discretePoint init;
    init.x = 25;
    init.y = 25;
    // double angle_init = 0.78; // pi/4
    double angle_init = 90; // pi/4
    double speed_init = 0.5;

    RobotTest bot(init, angle_init, speed_init);

    int timestep = 100;

    for(int i = 0; i < timestep; ++i)
    {
      std::cout<<i<<"th timestep: ";
      bot.move();
      // bot.talk();
      bot.draw();
    }
}
