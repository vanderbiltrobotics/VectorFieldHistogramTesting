//
// Created by Phil on 2/3/2018.
//
#ifndef VECTORFIELDHISTOGRAMTESTING_ROBOTTEST_CPP
#define VECTORFIELDHISTOGRAMTESTING_ROBOTTEST_CPP

#include <math.h>
#include <iostream>
#include "VFHPather.h"
#include "Utils.h"

class RobotTest{
private:
    VFHPather pather; //Object used to store the grid/map of obstacles
    discretePoint currentPosition; //Stores the location of the robot
    double currentSpeed; // linear distance per timestep
    double maxTurnSpeed; // angle per timestep
    double currentAbsoluteAngle; //absolute
    // double[][] world

public:
    //RobotTest
    //Constructor for the RobotTest class
    // init_x: (int)
    // init_y: (int)
    RobotTest(int init_x, int init_y, double angle_init, double speed_init):
    // pather(20, 50, 100, 0.1) // iMax = 10, jMax = 10??
        pather() // iMax = 10, jMax = 10??
    {
        currentPosition.x = init_x;
        currentPosition.y = init_y;
        currentAbsoluteAngle = angle_init;
        currentSpeed = speed_init;
        pather.updateRobotPosition(currentPosition);
    }

    ~RobotTest()
    {
        // delete pather; // FIXME: no delete method in them
        // delete currentPosition; // FIXME: no delete method in them
    }

    //main function per timestep
    void move()
    {
        updateRobotPosition();
        updateRobotAngle();
        updateRobotSpeed();
    }

    void updateRobotPosition()
    {
        currentPosition.x += currentSpeed * cos(currentAbsoluteAngle);
        currentPosition.y += currentSpeed * sin(currentAbsoluteAngle);
        std::cout<<"trying to update the robot position.\n";
        pather.updateRobotPosition(currentPosition);
        // pather.generateHistogram();
    }

    void updateRobotAngle()
    {
      // currentAbsoluteAngle = pather.getMinHistAngleBetweenAngles(currentPosition, currentAbsoluteAngle, maxTurnSpeed);
        std::cout<<"trying to update the robot angle.\n";
        currentAbsoluteAngle = pather.computeTravelDirection();
    }

    // TODO: currently speed is always 1 width per timestep
    void updateRobotSpeed()
    {
        currentSpeed = 1;
    }

    void talk() {
        std::cout<<"currentPosition is "<<currentPosition.x<<", "<<currentPosition.y<<"\n";
    }
  };
#endif //VECTORFIELDHISTOGRAMTESTING_ROBOTTEST_CPP
