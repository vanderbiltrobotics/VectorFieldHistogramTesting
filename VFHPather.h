//
// Created by Swapnil on 2/3/2018.
//

#ifndef VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H
#define VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H

#include "PolarHistogram.h"
#include "HistogramGrid.h"

class VFHPather{
private:

    PolarHistogram hist; //Object used to store the polar histogram
    HistogramGrid grid; //Object used to store the grid/map of obstacles
    contPoint robotLoc; //Stores the location of the robot

    double a;
    double b;
    //TODO Store the location fo the robot in the histogram grid rather in the pather

public:
    //TODO Provide fields to dynamically set these values
    VFHPather(): hist(20), grid(10, 10, 0.1)
    {}

    //TODO: Add ability to dynamically set certainty value
    //Using certainty of 1 for all object currently
    void updateRobotPosition(contPoint pos)
    {
        robotLoc = pos; //
    }


    //generateHistogram
    //Builds the vector field histogram based on current position of robot and surrounding obstacles
    void generateHistogram()
    {
        hist.reset();

        grid.getDiscretePointFromCont(robotLoc);
        region activeRegion = grid.getActiveRegion(robotLoc);

        discretePoint discRobotLocation = grid.getDiscretePointFromCont(robotLoc); //Discrete node for robot location
        discretePoint curNode; //Node currently being iterated over
        for(curNode.x = activeRegion.min.x; curNode.x < activeRegion.max.x; curNode.x++)
        {
            for(curNode.y = activeRegion.min.y; curNode.y < activeRegion.max.y; curNode.y++)
            {

                hist.addValue(grid.getAngle(discRobotLocation, curNode),
                        pow(grid.getCertainty(curNode),2)*(a-b*grid.getDistance(curNode, discRobotLocation)));

            }
        }

    }

};
#endif //VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H
