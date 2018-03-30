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

#define HIST_WIDTH 50 // originally 50
#define HIST_LENGTH 50 // originally 50
#define NODE_SIZE_IN 1
#define NUM_BINS 32 // 32 originally. 25 gets around to the target
#define ACTIVE_REGION_SIZE_I 30 // Originally 20. 30 gets stuck early on. 10 Goes through all obstacles.
#define ACTIVE_REGION_SIZE_J 30

// Less useful params
#define TARGET_X 50
#define TARGET_Y 50
#define A_IN 200
#define B_IN 1
#define L_IN 5
#define MAX_NUM_NODES_FOR_VALLEY_IN 15
#define VALLEY_THRESHOLD_IN 10

// #define MAP_FNAME "../map.txt"

/*
* NOTE: it is important to distinguish between the same variables at
* t versus t-1. Instance variables are shared across two timesteps.
*/

class RobotTest{
private:
    HistogramGrid grid;
    PolarHistogram hist;
    VFHPather pather; //Object used to store the grid/map of obstacles
    discretePoint location; //Stores the location of the robot
    double speed; // linear distance per timestep
    // double maxTurnSpeed; // angle per timestep REVIEW: necessary?
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
    RobotTest(discretePoint initPos):
        grid("../map.txt", initPos, ACTIVE_REGION_SIZE_I, ACTIVE_REGION_SIZE_J, HIST_WIDTH, HIST_LENGTH, NODE_SIZE_IN),
        hist(NUM_BINS),
        pather(hist, &grid, A_IN, B_IN, L_IN, MAX_NUM_NODES_FOR_VALLEY_IN, VALLEY_THRESHOLD_IN),
        location(initPos)
    {
        pather.updateRobotPosition(location);
        grid.setTargetLoc({TARGET_X, TARGET_Y});
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
