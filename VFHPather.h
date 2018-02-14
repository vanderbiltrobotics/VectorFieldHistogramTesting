//
// Created by Swapnil on 2/3/2018.
//

#ifndef VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H
#define VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H

#include <iostream>
#include <vector>


#include "PolarHistogram.h"
#include "HistogramGrid.h"


class VFHPather{
private:

    PolarHistogram hist; //Object used to store the polar histogram
    HistogramGrid grid; //Object used to store the grid/map of obstacles


    double a = 50; //Constants for the weighting function for histogram generation
    double b = 100;

    double valleyThreshold = 5;

public:
    //TODO Provide fields to dynamically set these values
//    VFHPather(): hist(20), grid(10, 10, 0.1)
//    {}

    VFHPather(): hist(true), grid(10, 10, 0.1)
    {}

    //TODO: Add ability to dynamically set certainty value
    //Using certainty of 1 for all object currently
    void updateRobotPosition(discretePoint pos)
    {
        grid.setRobotLoc(pos); //
        generateHistogram();
    }


    //generateHistogram
    //Builds the vector field histogram based on current position of robot and surrounding obstacles
    void generateHistogram()
    {
        hist.reset();
        // std::cout << "";
        region activeRegion = grid.getActiveRegion(grid.getRobotLoc());
        // std::cout << "4";
        // discretePoint curNode; //Node currently being iterated over

        std::cout << "Active Region: " << activeRegion.min.x << " "
                  << activeRegion.min.y << " " << activeRegion.max.x << " " << activeRegion.max.y << "\n";

        // for(curNode.x = activeRegion.min.x; curNode.x < activeRegion.max.x; curNode.x++)
        // {
        //     for(curNode.y = activeRegion.min.y; curNode.y < activeRegion.max.y; curNode.y++)
        //     {
        //         hist.addValue(grid.getAngle(grid.getRobotLoc(), curNode),
        //                 pow(grid.getCertainty(curNode),2)*(a-b*grid.getDistance(curNode, grid.getRobotLoc())));
        //     }
        // }
        for(int x = activeRegion.min.x; x < activeRegion.max.x; ++x)
        {
            for(int y = activeRegion.min.y; y < activeRegion.max.y; ++y)
            {
                discretePoint curNode;
                curNode.x = x;
                curNode.y = y;
                // std::cout<<"in inner loop: x=" << x << " y=" << y << "\n";
                discretePoint robotLoc = grid.getRobotLoc();
                double angle = grid.getAngle(robotLoc, curNode);
                std::cout<<"angle = "<< angle << "\n";
                double power = pow(grid.getCertainty(curNode),2);
                std::cout<<"power = "<< power << "\n";
                double distance = grid.getDistance(curNode, robotLoc);
                std::cout<<"distance = "<< distance << "\n";
                // hist.addValue(grid.getAngle(grid.getRobotLoc(), curNode), pow(grid.getCertainty(curNode),2)*(a-b*grid.getDistance(curNode, grid.getRobotLoc())));
                hist.addValue(angle, power * (a - b * distance));
            }
        }
    }

    //getIndex
    //Returns index 0 <= i < n that corresponds to a. "Wraps" a.
    int getIndex(int a, int n) {
        return ((a % n) + n) % n;
    }

    //TODO handle edge cases: There are no obstacles, target direction is in a valley
    //
    double computeTravelDirection()
    {
        //startBin represent bin at which valley finding starts
        int startBin = 3;//hist.getBinFromAngle(grid.getAngle(grid.getRobotLoc(), target)); //Determine the bin in which the target falls

        int negative = 1; //Used to alternate the direction of the array iteration
        int nearIndex; //Index of the edge of valley closest to the target
        int farIndex; //Index of the edge of the valley furthest from target

        std::cout<<"before the first loop";
        //Looping over the entire polar histogram
        for(int count = 1; count <= hist.getNumBins(); count = count + 1)
        {
            int i = getIndex(startBin + negative*count/2, hist.getNumBins());
            if(hist.getValue(i) < valleyThreshold) //Found valley
            {
                if(i == startBin)
                {
                    //Find upper boundary of valley
//                    while (count <= hist.getNumBins() && hist.getValue(i) < valleyThreshold)
//                    {
//                        i = getIndex(i + 1, hist.getNumBins());
//                    }
                    //Find lower boundary of valley
                    break;
                }
                else
                {
                    nearIndex = i;
                    count++;
                    i = getIndex(i + negative, hist.getNumBins()); //Adds or subtracts index based on direction of array traversal.

                    while (count <= hist.getNumBins() && hist.getValue(i) < valleyThreshold)
                    {
                        i = getIndex(i + negative, hist.getNumBins());
                    }
                    farIndex = i;
                    std::cout << nearIndex << " " << farIndex << std::endl;

                }
                break;
            }
            negative *= -1; //Flipping direction
        }
        int travelDirectionIndex;
        if(startBin > nearIndex && farIndex > nearIndex)
        {
            travelDirectionIndex = getIndex((nearIndex + farIndex + hist.getNumBins())/2, hist.getNumBins());
        }
        else if(startBin < nearIndex && farIndex < nearIndex)
        {
            travelDirectionIndex = getIndex((nearIndex - hist.getNumBins() + farIndex )/2, hist.getNumBins());
        }
        else
        {
            travelDirectionIndex = (nearIndex + farIndex)/2;
        }
        return travelDirectionIndex;
    }

    void printHistogram()
    {
        hist.printHistogram();
    }


};
#endif //VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H
