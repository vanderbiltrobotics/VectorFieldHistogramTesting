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
    HistogramGrid test("../map.txt");
    test.printGrid();
//    VFHPather pather;
//    discretePoint robotLoc;
//    robotLoc.x = 25;
//    robotLoc.y = 25;
//    pather.updateRobotPosition(robotLoc);
//    std:: cout << pather.computeTravelDirection();



}