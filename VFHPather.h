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

    int l = 5; //Smoothing constant for polar histogram

    double valleyThreshold = 15;

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
    }


    //generateHistogram
    //Builds the vector field histogram based on current position of robot and surrounding obstacles
    void generateHistogram()
    {
        hist.reset();
        std::cout << "3";
        region activeRegion = grid.getActiveRegion(grid.getRobotLoc());
        std::cout << "4";
        discretePoint curNode; //Node currently being iterated over

        std::cout << "Active Region: " << activeRegion.min.x << " "
                  << activeRegion.min.y << " " << activeRegion.max.x << " " << activeRegion.max.y << "\n";

        for(curNode.x = activeRegion.min.x; curNode.x < activeRegion.max.x; curNode.x++)
        {
            for(curNode.y = activeRegion.min.y; curNode.y < activeRegion.max.y; curNode.y++)
            {
                hist.addValue(grid.getAngle(grid.getRobotLoc(), curNode),
                        pow(grid.getCertainty(curNode),2)*(a-b*grid.getDistance(curNode, grid.getRobotLoc())));
            }
        }

    }

    //getIndex
    //Returns index 0 <= i < n that corresponds to a. "Wraps" a.
    int getIndex(int a, int n) {
        return ((a % n) + n) % n;
    }

    //TODO handle edge cases: There are no obstacles, target direction is in a valley
    //computeTravelDirection
    //Determines the optimal travel direction for the robot based on the generated polar histogram
    //Works by finding the valley whose direction most closely matches the direction of the target
    double computeTravelDirection()
    {
        //generateHistogram(); //Computes the polar histogram
        //hist.smoothHistogram(l); //Smoothing histogram

        //startBin represent bin at which valley finding starts
        int startBin = 1;//hist.getBinFromAngle(grid.getAngle(grid.getRobotLoc(), target)); //Determine the bin in which the target falls

        int negative = 1; //Used to alternate the direction of the array iteration
        int nearIndex = -1; //Index of the edge of valley closest to the target
        int farIndex = -1; //Index of the edge of the valley furthest from target

        //Determining if the target direction falls within a bin
        if(hist.getValue(startBin) < valleyThreshold) //Desired travel direction falls within a valley
        {
            std::cout << "test 1" << std::endl;
            //Find upper boundary of valley
            for(int i = startBin + 1; getIndex(i, hist.getNumBins()) != startBin; i++)
            {
                if(hist.getValue(i) > valleyThreshold)
                {
                    farIndex = i; //Found the further edge of the valley
                    break;
                }
            }
            //Was not able to find the edge of a valley. Therefore, there are no obstacles.
            //Robot should travel straight to target.
            if(farIndex == -1)
            {
                return hist.getAngleFromBin(startBin); //TODO return actual angle, not just index
            }

            //Find lower boundary of valley
            for(int i = startBin - 1; getIndex(i, hist.getNumBins()) != startBin; i--)
            {

                if(hist.getValue(getIndex(i, hist.getNumBins())) > valleyThreshold)
                {
                    nearIndex = i + 1; //Found the nearer edge of the valley
                    break;
                }
            }
            std::cout << nearIndex << " " << farIndex << std::endl;

            return getIndex((farIndex+nearIndex)/2, hist.getNumBins());

        }
        else {
            //Finding other valleys in histogram
            //Looping over the entire polar histogram
            //Count stores the number of bins that have been checked
            //Loop begins iterating at the startBin, and alternates back and forth to find nearest valley
            std::cout << "test 2" << std::endl;
            for (int count = 1; count <= hist.getNumBins(); count = count + 1) {
                int i = getIndex(startBin + negative * count / 2, hist.getNumBins()); //Index of bin to check next
                if (hist.getValue(i) < valleyThreshold) //Found valley
                {
                    nearIndex = i;
                    count++;
                    i = getIndex(i + negative, hist.getNumBins()); //Adds or subtracts index based on direction of array traversal.
                    std::cout << "test 3" << std::endl;
                    while (count <= hist.getNumBins() && hist.getValue(i) < valleyThreshold) {
                        i = getIndex(i + negative, hist.getNumBins());
                    }
                    farIndex = i;
                    std::cout << nearIndex << " " << farIndex << std::endl;

                    break; //Since loop begins iterating from the target direction, the valley must be the closest valley
                }
                negative *= -1; //Flipping direction
            }
        }

        //Edges of nearest valley are determined, must determine travel direction
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
        return travelDirectionIndex;//hist.getAngleFromBin(travelDirectionIndex);
    }

    void printHistogram()
    {
        hist.printHistogram();
    }


};
#endif //VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H
