#include <iostream>
#include "HistogramGrid.h"
#include "PolarHistogram.h"
#include "Utils.h"
#include "VFHPather.h"

int getIndex(int a, int n) {
        return ((a % n) + n) % n;
    }

int main()
{
    VFHPather pather;
    discretePoint robotLoc;
    robotLoc.x = 25;
    robotLoc.y = 25;
    pather.updateRobotPosition(robotLoc);
    //pather.generateHistogram();
    std:: cout << pather.computeTravelDirection();



}