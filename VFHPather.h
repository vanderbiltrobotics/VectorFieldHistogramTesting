//
// Created by Swapnil on 2/3/2018.
//

#ifndef VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H
#define VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H

#include <iostream>
#include "PolarHistogram.h"
#include "HistogramGrid.h"

class VFHPather{
private:

    PolarHistogram hist; //Object used to store the polar histogram
    HistogramGrid grid; //Object used to store the grid/map of obstacles
    discretePoint robotLoc; //Stores the location of the robot

    double a = 50;
    double b = 100;
    //TODO Store the location fo the robot in the histogram grid rather in the pather

public:
    //TODO Provide fields to dynamically set these values
    VFHPather(): hist(20), grid(10, 10, 0.1)
    {}

    //TODO: Add ability to dynamically set certainty value
    //Using certainty of 1 for all object currently
    void updateRobotPosition(discretePoint pos)
    {
        robotLoc = pos; //
    }


    //generateHistogram
    //Builds the vector field histogram based on current position of robot and surrounding obstacles
    void generateHistogram()
    {
        hist.reset();
        std::cout << "3";
        region activeRegion = grid.getActiveRegion(robotLoc);
        std::cout << "4";
        discretePoint curNode; //Node currently being iterated over

        std::cout << "Active Region: " << activeRegion.min.x << " "
                  << activeRegion.min.y << " " << activeRegion.max.x << " " << activeRegion.max.y << "\n";

        for(curNode.x = activeRegion.min.x; curNode.x < activeRegion.max.x; curNode.x++)
        {
            for(curNode.y = activeRegion.min.y; curNode.y < activeRegion.max.y; curNode.y++)
            {
                hist.addValue(grid.getAngle(robotLoc, curNode),
                        pow(grid.getCertainty(curNode),2)*(a-b*grid.getDistance(curNode, robotLoc)));
            }
        }

    }

    void printHistogram()
    {
        hist.printHistogram();
    }

};
#endif //VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H
