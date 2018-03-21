#include <iostream>
#include "HistogramGrid.h"
#include "PolarHistogram.h"
#include "Utils.h"
#include "VFHPather.h"
#include "RobotTest.h"

#include <chrono>
#include <thread>
#include "Plotter.h"


// BUG: 1. speed updating is not correct: why is it moving by 1 block every time?
// BUG: 2. simulation stopped at 98. Why?
// BUG: 3. robot going through obstacles. Why?

int getIndex(int a, int n) {
    return ((a % n) + n) % n;
}

int main()
{
    // Valley Hyperparameters
    int maxNumNodesForValley = 10;
    double a = 200;
    double b = 1;
    int l = 5;
    double valleyThreshold = 15;

    // Polar Hyperparameters
    int numBins = 16;
    int iSizeActiveRegion = 20;
    int jSizeActiveRegion = 20;
    int histWidth = 50;
    int histLength = 50;
    double nodeSize = 1;


    discretePoint initPosition;
    initPosition.x = 0;
    initPosition.y = 0;

    discretePoint targetPosition;
    targetPosition.x = 50;
    targetPosition.y = 50;

    RobotTest bot(initPosition, targetPosition, numBins, iSizeActiveRegion, jSizeActiveRegion, histWidth,
        histLength, nodeSize, maxNumNodesForValley, a, b, l, valleyThreshold);

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
