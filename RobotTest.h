// RobotTest.h
// @author {Zhanwen Chen, Swapnil Pande}
// @date Feb 3, 2018

#ifndef VECTORFIELDHISTOGRAMTESTING_ROBOTTEST_CPP
#define VECTORFIELDHISTOGRAMTESTING_ROBOTTEST_CPP

#include <math.h>
#include <iostream>
#include "VFHPather.h"
#include "Utils.h"
#include "Plotter.h"

/*
* NOTE: it is important to distinguish between the same variables at
* t versus t-1. Instance variables are shared across two timesteps.
*/

class RobotTest{
private:
    HistogramGrid grid;
    PolarHistogram polarHist;
    VFHPather pather; //Object used to store the grid/map of obstacles
    discretePoint location; //Stores the location of the robot
    double speed; // linear distance per timestep
    double angle; // In degrees.
    Plotter plotter;

    void draw(int currentTimestep, int numTimesteps)
    {
        int iMax = pather.getIMax();
        int jMax = pather.getJMax();
        std::vector<discretePoint> positions;
        positions.push_back(grid.getRobotLoc());
        positions.push_back(grid.getTargetLoc());
        for(int i = 0; i < iMax; ++i)
        {
            for(int j = 0; j < jMax; ++j)
            {
                if(pather.getCellValue(i, j) == 1)
                {
                    positions.push_back({i, j});
                }
            }
        }
        plotter.plot(positions, location.x, location.y, angle, currentTimestep, numTimesteps);
    }

    void getAngle()
    {
        angle = pather.computeTravelDirection();
        std::cout << "Desired travel angle: " << angle << "\n";
    }

    // TODO: currently speed is always 1 width per timestep
    void getSpeed()
    {
      speed = 2;
    }

    void updateLocation()
    {
        location.x += speed * cos(angle*M_PI/180);
        location.y += speed * sin(angle*M_PI/180);
        pather.updateRobotPosition(location);
    }


    void talk()
    {
        discretePoint targetLoc = grid.getTargetLoc();
        std::cout << "location is (" << location.x << ", "
                  << location.y << "); Desired Position is ("
                  << targetLoc.x << ", " << targetLoc.y << ")\n";
    }

public:
    //RobotTest
    //Constructor for the RobotTest class
    //CHANGED: no need for initAngle or initSpeed: robot should figure out.
    // RobotTest(discretePoint initPos, double initAngle, double initSpeed):
    RobotTest(discretePoint initPos, discretePoint targetPos, int numBins,
              int iSizeActiveRegion, int jSizeActiveRegion, int histWidth,
              int histLength, int nodeSize, int maxNumNodesForValley,
              double a, double b, int l, double valleyThreshold):
        grid("../map.txt", initPos, iSizeActiveRegion, jSizeActiveRegion, histWidth,
            histLength, nodeSize),
        polarHist(numBins),
        pather(polarHist, &grid, iSizeActiveRegion, jSizeActiveRegion, histWidth,
        histLength, nodeSize, maxNumNodesForValley, a, b, l, valleyThreshold),
        location(initPos)
    {
        pather.updateRobotPosition(location);
        grid.setTargetLoc({targetPos.x, targetPos.y});
    }


    //main function per timestep
    // 1. Get angle from nothing at t=0, then
    // 2. get speed from nothing at t=0.
    // 3. Given position at 0, draw simulation at t=0,
    // 4. Now move from t=0 to t=1 by only updating the robot's position.
    void move(int currentTimestep, int numTimesteps)
    {
        getAngle(); // angle: Null (or optionally, t-1) => t
        getSpeed(); // speed: Null (or optionally, t-1) => t

        draw(currentTimestep, numTimesteps); // at t=0, angle: t=0, speed, position: t

        updateLocation(); // position: t => t+1
    }
  };
#endif //VECTORFIELDHISTOGRAMTESTING_ROBOTTEST_CPP
