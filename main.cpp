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
    discretePoint init;
    init.x = 0;
    init.y = 0;

    // double angle_init = 0.78; // pi/4
    // double angle_init = 90; // pi/4
    // double speed_init = 0.5;

    RobotTest bot(init);
    getchar();

    int numTimesteps = 100;

    for(int i = 0; i < numTimesteps; ++i)
    {
        std::cout<<i<<"th timestep: ";
        bot.move(i, numTimesteps);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::cout << "\n\n\n\n\n\n";
    }
}
